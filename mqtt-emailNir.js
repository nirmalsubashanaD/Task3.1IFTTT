const mqtt = require("mqtt");
const nodemailer = require("nodemailer");

// MQTT Connection Settings
const brokerUrl = "mqtts://e0a61b4b45f149c78c01a7587c9deb7a.s1.eu.hivemq.cloud";
const topic = "sensor/light";
const client = mqtt.connect(brokerUrl, {
  username: "hivemq.webclient.1742519149295", 
  password: ";f!1*be,0ah29AQDCRIg", 
});

// Email Notification Setup
const transporter = nodemailer.createTransport({
  service: "gmail",
  auth: {
    user: "nirmalsubashana3@gmail.com", 
    pass: "nrwu ifjr djeu kvmf", 
  },
});

// Function to send email
const sendEmail = (subject, message) => {
  const mailOptions = {
    from: "nirmalsubashana3@gmail.com",
    to: "nirmalsubashanak@gmail.com", 
    subject: subject,
    text: message,
  };

  transporter.sendMail(mailOptions, (error, info) => {
    if (error) {
      console.log("Email Error: " + error);
    } else {
      console.log("Email Sent: " + info.response);
    }
  });
};

// MQTT Subscription
client.on("connect", () => {
  console.log("Connected to MQTT Broker!");
  client.subscribe(topic, (err) => {
    if (!err) {
      console.log(`Subscribed to topic: ${topic}`);
    } else {
      console.log("Subscription Error:", err);
    }
  });
});

// Handle Incoming MQTT Messages
client.on("message", (topic, message) => {
  const payload = message.toString();
  console.log(`Received message on topic ${topic}: ${payload}`);

  if (payload === "Sunlight Detected") {
    console.log("Sending email for sunlight detected...");
    sendEmail("Sunlight Alert", "Your terrarium is receiving sunlight.");
  } else if (payload === "Sunlight Stopped") {
    console.log("Sending email for sunlight stopped...");
    sendEmail("Sunlight Stopped", "Your terrarium is no longer in sunlight.");
  } else {
    console.log("No action for this message.");
  }
});

// Error handling
client.on("error", (err) => {
  console.error("MQTT Connection Error:", err);
});
