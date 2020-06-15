## 作成方法

### 環境
+ Windows 10 64bit 版
+ Visual Studio Community 2019
+ Qt5
    + インストールするコンポーネントで Qt 5.12.8 の MSVC 2017 64-bit を選択
        (TeamSpeak3 client が Qt 5.12.x 系列をリンクしているため)
+ Qt Visual Studio Tool
    + Visual Studio 内の「拡張機能と更新プログラム」からインストール
    + QtOptions で Qt\5.12.8\msvc2017_64 を msvc2017_64_Qt5.12.8 として登録

### 手順
1. この git リポジトリ (ts3plugins) をクローンする
2. フォルダ ts3plugins で git submodule update -i を実行する (TS3 Client Plugin SDK を取得する)
2. Developer Command Prompt for VS 2019 を起動する (msbuild へのパスを手動で通してもよい)
3. フォルダ ts3plugins\nyushitsu に移動する
4. msbuild nyushitsu_plugin.msbuild を実行する
5. フォルダ ts3plugins\nyushitsu\output に配布用パッケージ nyushitsu_plugin.zip が作成される

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
