#
# This is a picoweb example showing a web page route
# specification using view decorators (Flask style).
#
import picoweb


app = picoweb.WebApp(__name__)

@app.route("/")
def index(req, resp):
    yield from picoweb.start_response(resp)
    #yield from resp.awrite("I can show you a table of <a href='squares'>squares</a>.")
    htmlFile = open('html/html_1.html', 'r')
    for line in htmlFile:
        yield from resp.awrite(line)

#@app.route("/squares")
#def squares(req, resp):
#    yield from picoweb.start_response(resp)
#    yield from app.render_template(resp, "squares.tpl", (req,))


import ulogging as logging
logging.basicConfig(level=logging.INFO)

app.run(debug=True, host="192.168.1.7")
