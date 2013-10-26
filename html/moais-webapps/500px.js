var img = document.querySelector("#img");
var ajax = new XMLHttpRequest();
var key = "4uM5AwbCgyJQAyisqxMzwlLbyN2vOmeV90ZQWS1D";

setTimeout(imageUpdater, 50);

function imageUpdater() {
	var url = "api.500px.com:443/v1/photos.json?feature=popular&sort=rating&page=" + Math.floor(Math.random() * 100) + "&rpp=1&image_size=4&consumer_key=" + key;
	ajax.open("GET", "http://localhost/proxy/" + url, false);
	ajax.send(null);
	
	var data = JSON.parse(ajax.responseText);
	img.src = data.photos[0].image_url;

	setTimeout(imageUpdater, 3000);
}
