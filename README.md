# ZephyrのシェルでLチカ

ZephyrのシェルからLチカを行う.  
シェルに`toggle`と入力すると,Teensy 4.0上のLEDが点消灯する.  

## 動作環境
- ホスト: Windows11
- ターゲット: [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)
- Zephyrバージョン: commit 94829f444c42a316bf3eee97451c726ae421b38d

## 前提条件
Zephyrが[公式手順でインストール](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)されていること

## 実行方法
1. `zephyrprojct/`直下に本リポジトリをクローンする.
```
$git clone https://github.com/Mooan/zephyr-Lchica.git app
```
2. ビルドする.ビルド成果物は`build\zephyr\zephyr.hex`に生成される
```
$cd app
$west build -b teensy40
```

3. Teensy Loaderを[ダウンロード](https://www.pjrc.com/teensy/loader_win10.html)する.

4. Teensyボードのピン1はUART RX, とピン2はUART TXである。これをUART to USBに接続する.
  - [PJRC TEENSY 4](https://docs.zephyrproject.org/latest/boards/arm/teensy4/doc/index.html)詳細は参照

5. USBケーブルで、ホストのUSBポートとターゲットのUSBポートを接続し、2.の成果物を、3.のファイルを使って焼きこむ.

## TODO
やりたいこと

- [ ] [MCP2515](https://ww1.microchip.com/downloads/jp/DeviceDoc/21801D_JP.pdf)をSPIで制御する.
  - [ ] SPIは、[/* LPSPI4 MISO, MOSI, SCK, CS on Teensy-Pins 12/11/13/10 */](https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/arm/teensy4/teensy4-pinctrl.dtsi#L141) らしい. 
  - [ ] MCP2515の[SPIドライバ](https://github.com/zephyrproject-rtos/zephyr/blob/a1042c407964cd9710f544b8ba9a61c15a7de2fb/drivers/can/can_mcp2515.c#L1)が実装されている
  - [ ] MCP2515の[Shield](https://docs.zephyrproject.org/latest/boards/shields/mcp2515/doc/index.html)でやっていることを、`app.overlay`に書いてあげる
  - [ ] [CAN API](https://docs.zephyrproject.org/apidoc/latest/group__can__interface.html)を生やす
  - [ ] [サンプル](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/drivers/can/counter)を参考に通信プログラムを書いてみる
- [ ] Github ActionでCI対応する.
  - https://github.com/marketplace/actions/zephyr-west
- [ ] Qemuの仮想環境で動作するようにしてみる
- [ ] [ztest](https://docs.zephyrproject.org/latest/develop/test/ztest.html)を使ってみる
- [ ] ESP32にポーティングしてみる. 
  - 無線通信機能も動かしてみる.
- [ ] mruby/micropython/luaあたりを組み込んでみる
- [ ] Rustでコードを書いてみる
  - https://www.zephyrproject.org/embedding-rust-into-zephyr-firmware-using-c-bindgen/
    - Rustのstdランタイムはないのだろうか...
- [ ] Javascrioptのランタイムを試してみる
  - https://github.com/intel/zephyr.js
- [ ] [AWS IoT Device Embedded C SDK for Zephyr](https://github.com/aws/aws-iot-device-embedded-c-sdk-for-zephyr)を試してみる