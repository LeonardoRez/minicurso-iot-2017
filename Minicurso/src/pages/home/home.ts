import { Component } from '@angular/core';
import {Paho} from 'ng2-mqtt/mqttws31';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  client;//objeto que faz comunicação com o broker
  led = "Nenhuma informação recebida sobre o led"
  constructor() {
    this.client = new Paho.MQTT.Client('m13.cloudmqtt.com', 37093, 'qwerty12345');
    var options = {
      useSSL: true,
      userName: "cdhwvxkk",
      password: "uoHN9SWskIFf",
      onSuccess: this.onConnected.bind(this)
    }
    this.onMessage();
    this.onConnectionLost();
    this.client.connect(options);
  }

  onConnected() {
    console.log("Connected");
    this.client.subscribe("/led/estado/");
  }

  sendMessage(message: string, destination: string) {
    let packet = new Paho.MQTT.Message(message);
    packet.destinationName = destination;
    this.client.send(packet);
  }

  onMessage() {
    this.client.onMessageArrived = (message: Paho.MQTT.Message) => {
      console.log('Message arrived : ' +message.destinationName+"   "+ message.payloadString);
      switch(message.destinationName){
        case "/led/estado/":{
          if(message.payloadString == "ligado")
            this.led = "O led está ligado"
          else if(message.payloadString == "desligado")
            this.led = "O led está desligado"
          break;
        }
      }

      }

  }

  onConnectionLost() {
    this.client.onConnectionLost = (responseObject: Object) => {
      console.log('Connection lost : ' + JSON.stringify(responseObject));
    };
  }
  desligaLed(){
    this.sendMessage("desligar","/led/comando/");
  }
  ligaLed(){
    this.sendMessage("ligar","/led/comando/");

  }


}
