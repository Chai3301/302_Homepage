<> #これはタグ
html= Hyper-Text Markup Language of Abbreviation
練習用に作ったもの



Cursor のコミット UI が落ちるときは、メッセージを -m で渡すのが定石です。エディタを開かないので、今回のエラーを避けられます。

手順（PowerShell）
変更をステージする（まだなら）

git add .
特定ファイルだけなら:

git add works/kamitana.html assets/css/style.css
メッセージ付きでコミット
git commit -m "ここにコミットメッセージ"
複数行にしたいとき（PowerShell）:

git commit -m @"
1行目のメッセージ
必要なら2行目
"@
確認
git status
よく使う流れ
git status
git diff
git add .
git commit -m "Update kamitana documents toggle"
git status
push するときだけ:

git push
なぜ UI だと失敗するか
Cursor はコミット時に内部エディタ（git-editor.sh）を開こうとします。その接続が切れると:

Bad status code: 500
ENOENT \\.\pipe\vscode-git-...
が出て、最後に Please supply the message using either -m or -F option と出ます。
-m を付ければそのエディタを使わないので成功します。

メッセージの書き方の目安
このリポジトリだと短くて十分です。

Add washinokasa gallery images
Update kamitana documents with From Y memo toggle
「何を入れたか / 何を直したか」が分かれば OK です。

ターミナルで git commit -m "..." を使う習慣にすれば、この手のエラーではこちらに頼らなくて大丈夫です。push や PR 作成だけ任せたいときも、その旨を言ってもらえれば対応できます。