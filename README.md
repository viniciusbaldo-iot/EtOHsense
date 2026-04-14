Sistema de monitoramento de vapores de alcool em ambiente controlado📌 Descrição Sistema IoT para monitoramento e controle de gases, utilizando ESP32, com envio de dados para nuvem (AWS) e visualização em tempo real via Node-RED.

🧱 Arquitetura ESP32 → WiFi → AWS → (HTTP + MQTT) → Node-RED Dashboard

⚙️ Tecnologias • ESP32 • Arduino IDE • Node-RED • MQTT (Mosquitto) • HTTP (PHP) • MySQL • AWS EC2

🔌 Hardware • ESP32 N16R8 • OLED SSD1306 (I2C) • Sensor MQ-3 • LEDs (status) • Buzzer • Resistores

🔗 Funcionalidades • Leitura de vapores de alcool no ambiente • Exibição em display OLED • Envio de dados via HTTP • Envio de dados via MQTT • Dashboard em tempo real

🌐 Backend 📡 API HTTP Arquivo: server/dados.php 

🗄️ Banco de Dados Arquivo: database/banco.sql Tabela: • leituras o id o umidade o nivel o data_hora

📥 Backup do Banco No servidor: mysqldump -u usuario -p irrigacao > banco.sql

📊 Dashboard O dashboard apresenta: • Nivel de vapor de alcool no ambiente em ppm • Status do ambiente  • Status do alarme

🚀 Execução ESP32 • Configurar WiFi • Upload via Arduino IDE Node-RED • Importar flow.json • Configurar broker MQTT Backend • Subir dados.php no servidor • Criar banco com banco.sql

📷 Demonstração Ver pasta /imagens

🔬 Simulação (Wokwi)

https://wokwi.com/projects/457159622309992449

👩‍💻 Autor: Vinicius Baldo Rocha Projeto acadêmico de IoT aplicado a irrigação inteligente.
