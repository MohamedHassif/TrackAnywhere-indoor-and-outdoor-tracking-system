from flask import Flask,render_template
site_url = "http://127.0.0.1:5000/"
webapp=Flask(__name__)
@webapp.route('/')
def home():
    return render_template('index.html')
@webapp.route('/<data>')
def datas(data):
    return print("This Your data",data)
webapp.run(host='0.0.0.0')


