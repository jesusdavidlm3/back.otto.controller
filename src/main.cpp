#include <WiFi.h>
#include <WebServer.h>
#include <Otto.h>

enum {
	WIFI_TIMEOUT = 5000,
	WIFI_CHECK = 10000,
	SERIAL_RATE = 9600,
	SERVO_RIGHT_LEG,
	SERVO_RIGHT_FEET,
	SERVO_LEFT_LEG,
	SERVO_LEFT_FEET,
	BUZZER,
	OTTO_STEP = 1,
	OTTO_TIME = 1000,
};
const char wifi_ssid[] = "Dubuc";
const char wifi_pass[] = "malena_160664";
unsigned long last_wifi_time = 0;
WebServer server(80);
Otto robot;

void test_connection() {
	server.send(200);
}

void set_move_state() {
	if(robot.getRestState())
		robot.setRestState(false);
}

void forwards_route() {
	set_move_state();
	robot.walk(OTTO_STEP, OTTO_TIME, FORWARD);
	server.send(200);
}
void backwards_route() {
	set_move_state();
	robot.walk(OTTO_STEP, OTTO_TIME, BACKWARD);
	server.send(200);
}
void left_route() {
	set_move_state();
	robot.turn(OTTO_STEP, OTTO_TIME, LEFT);
	server.send(200);
}
void right_route() {
	set_move_state();
	robot.turn(OTTO_STEP, OTTO_TIME, RIGHT);
	server.send(200);
}
void stop() {
	robot.home();
	robot.setRestState(true);
	server.send(200);
}

void print_ip() {
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);
}

void connect() {
	bool start = true;
	while(true) {
		WiFi.disconnect();
		if (!start)
			delay(WIFI_TIMEOUT);
		if(start)
			start = false;

		Serial.print("Trying to Connect to ");
		Serial.println(wifi_ssid);
		WiFi.begin(wifi_ssid, wifi_pass);
		unsigned long start = millis();
		int wifi_status;
		while((wifi_status = WiFi.status()) != WL_CONNECTED && millis() - start < WIFI_TIMEOUT)
			delay(1);

		Serial.print("Status: ");
		Serial.println(wifi_status);
		if (wifi_status != WL_CONNECTED)
			continue;

		Serial.println("Succesfully connected!");
		print_ip();

		last_wifi_time = millis();
		return;
	}
}

void map_server() {
	server.on("/otto/fordward", forwards_route);
	server.on("/otto/backward", backwards_route);
	server.on("/otto/left", left_route);
	server.on("/otto/right", right_route);
	server.on("/otto/stop", stop);
	server.on("/otto/test_connection", test_connection);
}

void setup() {
	// Init Serial Output
	Serial.begin(SERIAL_RATE);
	while(!Serial);
	robot.init(SERVO_LEFT_LEG, SERVO_RIGHT_LEG, SERVO_LEFT_FEET, SERVO_RIGHT_FEET, true, BUZZER);
	robot.home();
	// NOTE: Codigo para inicializar la pantalla me imagino.
	// robot.initMATRIX(DATA_IN_PIN, CS_PIN, CLK_PIN, MATRIX_ORIENTATION);
	connect();
	map_server();
	server.begin();
}

void loop() {
	unsigned long now = millis();
	if (now - last_wifi_time > WIFI_CHECK) {
		Serial.println("Checking WiFi connection...");
		int status = WiFi.status();
		Serial.print("Status: ");
		Serial.println(status);
		if (status != WL_CONNECTED) {
			server.stop();
			connect();
			server.begin();
		}
		last_wifi_time = now;
	}
	server.handleClient();
}
