// globals
var GameStatus = {
	waiting: "Ожидание",
	ingame: "В игре",
	win: "Победа",
	loose: "Поражение"
};
var MapSize, Canvas, Context;

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
	$("#id_self").text(msg.id);
	$("#status").text(GameStatus.waiting);
}

function enemy_info(msg) {
	$("#id_enemy").text(msg.id);
	$("#text_enemy").removeClass("hidden");
	$("#status").text(GameStatus.waiting);
}

function game_info(msg) {
	MapSize = {
		width: msg.world.width,
		height: msg.world.height,
		//ratio: msg.world.width / msg.world.height
	};
	
	var Box = $("#canvas_box");
	
	var map_r = MapSize.width / MapSize.height;
	var canv_r = Box.width() / Box.height();	
	if (map_r > canv_r) {
		Canvas = $("<canvas width='" + Box.width() + "' height='" + Box.width() / map_r + "'></canvas>");
		$(window).resize(function() {
			Canvas.width(Box.width());
			Canvas.height(Box.width() / map_r);
		});
	} else {
		Canvas = $("<canvas width='" + Box.height() * map_r + "' height='" + Box.height() + "'></canvas>");
		$(window).resize(function() {
			console.log($(window).height()); // WTF?? почему размер не уменьшается???
			Canvas.width(Box.height() * map_r);
			Canvas.height(Box.height());
		});
	}
	
	Canvas.appendTo(Box);	
	Context = Canvas.get(0).getContext("2d");
	Context.fillRect(0, 0, Canvas.width(), Canvas.height());
}

function action(msg) {
	window["action_" + msg.action](msg);
}

function action_start(msg) {
	$("#status").text(GameStatus.ingame);
}

function action_finish(msg) {
	if ($("#id_self").text() == msg.winner)
		$("#status").text(GameStatus.win);
	else 
		$("#status").text(GameStatus.loose);
}

function action_update(msg) {
	Context.fillRect(0, 0, Canvas.width(), Canvas.height());
	for (var i in msg.mapelements) {
		i = msg.mapelements[i];
		var me = new MapElement(i.position.x, i.position.y, i.width, i.height, i.angle, i.rotationcenter.x, i.rotationcenter.y);
		me.draw();
	}
}


function translateCoords(c) {
	c.x = c.x / MapSize.width * Canvas.width();
	c.y = c.y / MapSize.height * Canvas.height();
	return c;	
}


function MapElement(x, y, width, height, rx, ry, a) {
	this.pos = {x: x, y: y};
	this.size = {x: width, y: height};
	this.rot = {x: rx, y: ry, a: a};
	this.color = "#351EFF";
	
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