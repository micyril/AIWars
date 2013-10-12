var socket;
var status = {
	waiting: "Ожидание",
	ingame: "В игре",
	win: "Победа",
	loose: "Поражение"
};

$(function() {
	socket = new WebSocket("ws://" + location.hostname);
	socket.onmessage = function (event) {
		var msg = JSON.parse(event.data);
		window[msg.mtype](msg);
	};
	socket.onopen = function() { 
		//alert("Соединение установлено."); 
	};
	socket.onclose = function(event) { 
		if (event.wasClean) {
			alert('Соединение закрыто');
		} else {
			alert('Обрыв соединения');
		}
	};
	socket.onerror = function(error) { 
		alert("Ошибка " + error.message); 
	};	
})

function self_info(msg) {
	$("#id_self").html(msg.id);
	$("#status").html(status.waiting);
}

function enemy_info(msg) {
	$("#id_enemy").html(msg.id);
	$("#text_enemy").removeClass("hidden");
	$("#status").html(status.waiting);
}

function action(msg) {
	switch (msg.action) {
		case "start":
			$("#status").html(status.ingame);
			break;
	}
}