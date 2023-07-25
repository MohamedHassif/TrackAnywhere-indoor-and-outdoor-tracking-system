function initMap(){

    var options={
        center:{lat:38.3460,lng:-0.4907},
        zoom:8
    }
    map=new google.maps.Map(document.getElementById("map"),options)
    /*
    const marker =new google.maps.Marker({
    position:{lat:37.9922,lng:-1.1307},
    map:map
    });

    const detailWindow = new google.maps.InfoWindow({
        content: `<h2>Murcia City</h2>`
    });

    marker.addListener("mouseover",() =>{   //onload-for showing location,click-if you touch that Location will show
        detailWindow.open(map,marker);
    })
*/
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