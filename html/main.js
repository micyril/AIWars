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
		console.log(msg.mtype);
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

function game_info(msg) {
	map_size = {
		width: msg.width,
		height: msg.height,
		ratio: msg.width / msg.height
	};
	
	canvas = $("#canvas");		
	var map_r = map_size.width / map_size.height;
	var canv_r = canvas.width() / canvas.height();	
	if (map_r > canv_r) {
		canvas.width("100%");
		canvas.height(canvas.width() / map_r);
		$(window).resize(function() {
			canvas.height(canvas.width() / map_r);
		});
	} else {
		canvas.height("100%");
		canvas.width(canvas.height() * map_r);
		$(window).resize(function() {
			canvas.width(canvas.height() * map_r);
		});
	}
	ctx = canvas.get(0).getContext("2d");
	ctx.fillRect(0, 0, canvas.width(), canvas.height());
}

function action(msg) {
	switch (msg.action) {
		case "start":
			$("#status").html(status.ingame);
			break;
	}
}

function translateCoords(c) {
	c.x = c.x / map_size.width * canvas.width();
	c.y = c.y / map_size.height * canvas.height();
	return c;	
}