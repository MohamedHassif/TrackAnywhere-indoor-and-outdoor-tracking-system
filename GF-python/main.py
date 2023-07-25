from flask import  Flask,render_template,request

site_url = "http://127.0.0.1:5000/"
webapp = Flask(__name__)
lat=0.0
lon=0.0
@webapp.route('/')
def home():
    return render_template('index.html')

@webapp.route('/<lat>/<lon>',methods=['GET','POST'])
def datas(lat,lon):
    print("Latitude==",lat,"---","Longitude==",lon)
    return render_template('index.html',lat=lat,lng=lon)
webapp.run(host='0.0.0.0')