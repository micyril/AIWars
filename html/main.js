onload = websocket_test;

function websocket_test() {
	var socket = new WebSocket("ws://127.0.0.1");
	socket.onopen = function() { 
		alert("Соединение установлено."); 
	};

	socket.onclose = function(event) { 
		if (event.wasClean) {
			alert('Соединение закрыто чисто');
		} else {
			alert('Обрыв соединения');
		}
	};
	 
	socket.onmessage = function(event) {
		alert("Получены данные " + event.data);
	};

	socket.onerror = function(error) { 
		alert("Ошибка " + error.message); 
	};
}
