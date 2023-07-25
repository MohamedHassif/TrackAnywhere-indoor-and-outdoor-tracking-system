function initMap(){

    var options={
        center:{lat:38.3460,lng:-0.4907},
        zoom:8
    }
    map=new google.maps.Map(document.getElementById("map"),options)
function addMarker(property){
    const marker = new google.maps.Marker({
        position:property.location,
        map:map,
        icon:"icons8-google-maps-40.png"
    });
}
    addMarker({location:{lat:37.9922,lng:-1.1307}});
    addMarker({location:{lat:39.4699,lng:-0.3763}});
    addMarker({location:{lat:38.5411,lng:-0.1225}});
}