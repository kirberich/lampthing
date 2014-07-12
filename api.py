import json

from twisted.web import http
from twisted.web.http import HTTPChannel
from twisted.internet import reactor
import threading

class BotHandler(http.Request, object):
    def __init__(self, api, *args, **kwargs):
        self.api = api
        super(BotHandler, self).__init__(*args, **kwargs)

    def render(self, content, headers):
        for (header_name, header_value) in headers:
            self.setHeader(header_name, header_value)
        self.write(content)
        self.finish()

    def simple_render(self, content, content_type="text/plain"):
        self.render(content, [("Content-Type", content_type)])

    def not_found(self, message=None):
        self.setResponseCode(404, message)
        return self.simple_render("no no...")

    def process(self):
        path = [x for x in self.path.split("/") if x]
        try:
            if not path:
                f = open("main.html")
                content = f.read()
                return self.simple_render(content, content_type="text/html")
            elif path[0] == "set" and self.method == 'POST':
                args = json.loads(self.content.read())
                command = args['command']
                lamp = args['lamp']
                remote = args['remote']
                self.api.trigger("set", command=command, lamp=lamp, remote=remote)
                return self.simple_render("set.")
        except Exception, e:
            return self.simple_render(e.message)

        return self.not_found()


class BotHandlerFactory(object):
    def __init__(self, api):
        self.api = api

    def __call__(self, *args, **kwargs):
        return BotHandler(self.api, *args, **kwargs)


class StreamFactory(http.HTTPFactory):
    protocol = HTTPChannel


class Api:
    def __init__(self):
        HTTPChannel.requestFactory = BotHandlerFactory(api=self)
        self.events = []

    def demonize(self, port=8080):
        reactor.listenTCP(port, StreamFactory())
        t = threading.Thread(target=reactor.run)
        t.daemon = True
        t.start()

    def run(self, port=8080):
        reactor.listenTCP(port, StreamFactory())
        reactor.run()

    def trigger(self, event, **kwargs):
        for x in range(len(self.events)):
            if self.events[x][0] == event:
                self.events[x] = (event, kwargs)
                return
        self.events.append((event, kwargs))
                                              
