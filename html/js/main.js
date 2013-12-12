var BORDER_WIDTH = 10;
var SELFID, ENEMYID, STATUS;

// globals
var GameStatus = {
	waiting: "Ожидание",
	ingame: "В игре",
	win: "Победа",
	loose: "Поражение"
};
var MapSize;
var Padding;
var SVG, jSVG;


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
		console.log("Ошибка " + error.message); 
	};
})

function self_info(msg) {
	SELFID = msg.id;
	STATUS = GameStatus.waiting;
	setPlane("id_self", msg.id, "Ваш ID");
	setPlane("status", GameStatus.waiting, "Статус");
}

function enemy_info(msg) {
	ENEMYID = msg.id;
	STATUS = GameStatus.waiting;
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
	var Box = $("#field_box");
	Padding = {
		width: innerWidth - Box.width(),
		height: innerHeight - Box.height()
	};

	var map_r = MapSize.ratio;
	var field_r = Box.width() / Box.height();
	var w, h;
	if (map_r > field_r) {
		w = (Box.width() - BORDER_WIDTH*2);
		h = (Box.width() / map_r - BORDER_WIDTH*2);
	} else {
		w = (Box.height() * map_r - BORDER_WIDTH*2);
		h = (Box.height() - BORDER_WIDTH*2);
	}
	Box.svg();
	SVG = Box.svg('get');
	jSVG = $(SVG._svg);
	jSVG.attr("id", "field");
	jSVG.width(w);
	jSVG.height(h);

	$(window).resize(resizeField);

	SVG.prepare = function() {
		SVG.clear();
		SVG.rect(0, 0, "100%", "100%", {fill: "black", id: "background"});
	}
	SVG.prepare();
}

function action(msg) {
	window["action_" + msg.action](msg);
}

function action_start(msg) {
	STATUS = GameStatus.ingame;
	setPlane("status", GameStatus.ingame);
}

function action_finish(msg) {
	if ($("#id_self").find(".value").text() == msg.winner) {
		STATUS = GameStatus.win;
		setPlane("status", GameStatus.win, "Статус");
	} else {
		STATUS = GameStatus.loose;
		setPlane("status", GameStatus.loose, "Статус");
	}
}

function action_update(msg) {
	SVG.prepare();
	for (var i in msg.mapelements) {
		i = msg.mapelements[i];
		var me = new MapElement(i.position.x, i.position.y, i.width, i.height, i.rotationcenter.x, i.rotationcenter.y, i.angle, i.robotId, i.type);
		me.draw();
	}
}


function translateCoords(c) {
	return {
		x: c.x / MapSize.width * jSVG.width(),
		y: c.y / MapSize.height * jSVG.height()
	};
}

function MapElement(x, y, width, height, rx, ry, a, rid, type) {
	this.pos = {x: x, y: y};
	this.size = {x: width, y: height};
	this.rot = {x: rx, y: ry, a: a};
	this.type = type;
	if (this.type == "RobotFrame") {
		this.type += (rid == SELFID ? "0" : "1");
	}
	
	this.draw = function() {
		var p = translateCoords(this.pos);
		var r = translateCoords(this.rot);
		var sz = translateCoords(this.size);

		r.a = -this.rot.a * 180/Math.PI;
		var g = SVG.group({
			transform: "rotate("+r.a+" "+r.x+" "+r.y+") translate("+p.x+" "+p.y+")",
			"class": this.type
		});
		SVG.rect(g, 0, 0, sz.x, sz.y).original = this;
	}
}

function resizeField() {
	var area = {
		width: innerWidth - Padding.width,
		height: innerHeight - Padding.height
	};
	var map_r = MapSize.ratio;
	var field_r = area.width / area.height;
	var w, h;	
	if (map_r > field_r) {
		w = area.width - BORDER_WIDTH*2;
		h = area.width / map_r - BORDER_WIDTH*2; 
	} else {
		w = area.height * map_r - BORDER_WIDTH*2;
		h = area.height - BORDER_WIDTH*2;
	}
	jSVG.width(w).height(h);
	jSVG.find("*").each(function(i, e) {
			if (e.original) {
				var o = e.original;
				e.remove();
				o.draw();
			}
		});
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

function Primitive(url) {
	var img = new Image();
	img.src = url;
	return img;
}