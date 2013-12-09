var BORDER_WIDTH = 10;

// globals
var GameStatus = {
	waiting: "Ожидание",
	ingame: "В игре",
	win: "Победа",
	loose: "Поражение"
};
var RobotColor = [
	"#9c1dce",
	"#9cbcbc",
	"#de5f4b",
	"#a0a525",
	"#365b0d",
	"#a8047c",
	"#5ce95a",
	"#8d7b5c",
	"#3e8efb",
	"#b7a5aa"
];
var MapSize, Canvas, Context;
var Padding;
var TempCanvas = $("<canvas></canvas>");
var TempContext = TempCanvas.get(0).getContext("2d");


$(function() {
	socket = new WebSocket("ws://" + location.hostname);
	socket.onmessage = function (event) {
		var msg = JSON.parse(event.data);
		console.log(msg);
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
	setPlane("id_self", msg.id, "Ваш ID");
	setPlane("status", GameStatus.waiting, "Статус");
}

function enemy_info(msg) {
	setPlane("id_enemy", msg.id, "ID соперника");
	$("#status").remove();
	setPlane("status", GameStatus.waiting, "Статус");
}

function game_info(msg) {
	MapSize = {
		width: msg.world.width,
		height: msg.world.height,
		ratio: msg.world.width / msg.world.height
	};	
	var Box = $("#canvas_box");
	Padding = {
		width: innerWidth - Box.width(),
		height: innerHeight - Box.height()
	};

	var map_r = MapSize.ratio;
	var canv_r = Box.width() / Box.height();
	var w, h;
	if (map_r > canv_r) {
		w = (Box.width() - BORDER_WIDTH*2);
		h = (Box.width() / map_r - BORDER_WIDTH*2);
	} else {
		w = (Box.height() * map_r - BORDER_WIDTH*2);
		h = (Box.height() - BORDER_WIDTH*2);
	}
	Canvas = $("<canvas width='" + w + "' height='" + h + "'></canvas>");
	Box.empty();
	Box.append(Canvas);

	$(window).resize(resizeCanvas);

	Context = Canvas.get(0).getContext("2d");
	Context.clear = function() {
		this.fillStyle = "#000000";
		this.fillRect(0, 0, Canvas.width(), Canvas.height());
	}
	Context.clear();
}

function action(msg) {
	window["action_" + msg.action](msg);
}

function action_start(msg) {
	setPlane("status", GameStatus.ingame);
}

function action_finish(msg) {
	if ($("#id_self").find(".value").text() == msg.winner)
		setPlane("status", GameStatus.win, "Статус");
	else 
		setPlane("status", GameStatus.loose, "Статус");
}

function action_update(msg) {
	Context.clear();
	for (var i in msg.mapelements) {
		i = msg.mapelements[i];
		var me = new MapElement(i.position.x, i.position.y, i.width, i.height, i.rotationcenter.x, i.rotationcenter.y, i.angle, i.robotId);
		me.draw();
	}
}


function translateCoords(c) {
	c.x = c.x / MapSize.width * Canvas.width();
	c.y = c.y / MapSize.height * Canvas.height();
	return c;
}

function MapElement(x, y, width, height, rx, ry, a, rid) {
	this.pos = {x: x, y: y};
	this.size = {x: width, y: height};
	this.rot = {x: rx, y: ry, a: a};
	this.color = RobotColor[rid];
	
	this.draw = function() {
		var pos = translateCoords(this.pos);
		var rot = translateCoords(this.rot);
		var sz = translateCoords(this.size);
		
		Context.fillStyle = this.color;
		Context.translate(rot.x, rot.y);
		Context.rotate(-rot.a);
		Context.fillRect(pos.x - rot.x, pos.y - rot.y, sz.x, sz.y);
		Context.rotate(rot.a);
		Context.translate(-rot.x, -rot.y);
	}
}

function resizeCanvas() {
	var area = {
		width: innerWidth - Padding.width,
		height: innerHeight - Padding.height
	};
	var map_r = MapSize.ratio;
	var canv_r = area.width / area.height;
	var w, h;	
	if (map_r > canv_r) {
		w = area.width - BORDER_WIDTH*2;
		h = area.width / map_r - BORDER_WIDTH*2; 
	} else {
		w = area.height * map_r - BORDER_WIDTH*2;
		h = area.height - BORDER_WIDTH*2;
	}
	// тупой канвас - ресайзится через жопу!
	//Canvas.width(w);
	//Canvas.height(h);
	TempCanvas.attr("width", w);
	TempCanvas.attr("height", h);
	TempContext.drawImage(Canvas.get(0), 0, 0, w, h);
	Canvas.attr("width", w);
	Canvas.attr("height", h);
	Context.drawImage(TempCanvas.get(0), 0, 0, w, h);
}

// создает новую информационную плашку или обновляет значение в существующей
function setPlane(id, v, k) {
	var plane = $("#" + id);
	if (plane.length != 0) {
		if (k) plane.find(".key").text(k);
		if (v) plane.find(".value").text(v);
	} else {
		plane.remove();
		plane = $("#info_plane_original").clone();
		plane.find(".key").text(k);
		plane.find(".value").text(v);
		plane.prop("id", id);
		plane.removeClass("hidden");
		$("#info_panel").append(plane);
	}
}
