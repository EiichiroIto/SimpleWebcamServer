# SimpleWebcamServer
Simple Webcam Server for ESP32

カメラモジュールのついたESP32デバイス（いわゆるESP32 Camera）を無線ウェブカメラにするサンプルプログラムです。M5Stackのgithub（ https://github.com/m5stack/M5Stack-Camera/tree/master ）にあるサンプルプログラムをもとに、できるだけ不要なコードを減らしていった（つもりの）ものです。

以下が特徴です。

- MJPEG 形式でビデオのストリーミングを行います。
- アクセスポイントとして動作します（APモード）
- 既存のアクセスポイントに接続して動作します（STAモード）

デフォルトではアクセスポイントとして動作します。SimpleWebcamServer というアクセスポイントに接続し、 http://192.168.4.1 を開けばストリーミングが始まります。

カメラの設定やウェブの設定は config.h で指定した上でビルドします。
config.h ファイルを見ればなんとなく設定方法がわかるでしょう。
利用するESP32カメラの種類は、cam_config.h を参照してください。

ビルドには Arduino-IDE を使います。Espressif の提供する esp32 ライブラリをインストールしてください。
参照→ ( https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide )

ビルドオプションは以下を指定する必要があります。

- ツール - ボード - esp32 - ESP32 Wrover Module
- ツール - Partition Scheme - Huge APP

