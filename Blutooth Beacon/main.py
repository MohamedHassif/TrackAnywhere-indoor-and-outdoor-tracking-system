from flask import  Flask,render_template,request
site_url = "http://127.0.0.1:5000/"
webapp = Flask(__name__)
da = "No value detected"
@webapp.route('/')
def home():
    return  render_template('home.html')
@webapp.route('/<data>')
def datas(data):
    global da
    da = "No value detected"
    print(da)
    return render_template('home.html',status=da)
webapp.run(host='0.0.0.0')
