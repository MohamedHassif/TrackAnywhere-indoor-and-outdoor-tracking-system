from flask import Flask, request

app = Flask(_name_)

@app.route('/gps', methods=['POST'])
def receive_gps_data():
    latitude = request.form.get('latitude')
    longitude = request.form.get('longitude')
    print(f'Latitude: {latitude}, Longitude: {longitude}')
    # Do something with the latitude and longitude data here
    return 'Data received successfully'

if _name_ == '_main_':
    app.run(debug=True)