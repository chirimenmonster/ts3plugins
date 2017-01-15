## 作成方法

### 環境
+ Windows 10 Home 64bit 版
+ Visual Studio 2015 community
 + Visual Studio 2015 Software Development Kit (SDK) (インストールオプション)
 + Modeling SDK for Microsoft Visual Studio 2015 (別途ダウンロードしてインストール)
 + MSBuild Extension Pack (別途ダウンロードしてインストール)
 + Qt5 Package (Visual Studio 内の「拡張機能と更新プログラム」からダウンロード)
+ Qt 5.7

### 手順
1. ファイルを適当なフォルダに展開する
2. 「開発者コマンドプロンプト」を起動する (msbuild へのパスを手動で通してもよい)
3. フォルダツリーの ts3plugins\nyushitsu に移動する
4. msbuild nyushitsu_plugin.msbuild を実行する
5. output フォルダに配布用パッケージ nyushitsu_plugin.zip が作成される

```
output
 +-- build                               コンパイル結果 (DLL など)
 +-- distribution                        配布パッケージの内容物
 |    +-- nyushitsu_plugin.ts3_plugin    TS3プラグインのインストールパッケージ (package フォルダを zip でまとめたもの)
 +-- package                             TS3プラグイン内部の構成
 |    +-- package.ini                    TS3プラグインのパッケージ情報
 |    +-- plugins
 |         +-- nyushitsu_plugin.dll      TS3プラグイン本体
 +-- nyushitsu_plugin.zip                配布パッケージ
```
