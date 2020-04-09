# 競賽幫手

## 安裝

``` sh
git clone https://github.com/MROS/contest_helper
cd contest_helper
cargo install --path . # 編譯並安裝
mkdir -p ~/.config/ch  # 建立設定目錄
ln -s $(pwd)/template ~/.config/ch/template # 鏈接模板，如果不想用鏈結的，也可以複製整個目錄
```

## 設定（未完成）

您可以在 `~/.config/ch/config.toml` 中設定，說明如下

``` toml
# 預設語言
default-lang = "cpp"
# 開目錄時的檔案名
dir = ["A", "B", "C", "D", "E", "F", "G", "H"]
```

## 使用

```
# 創造一個 C++ 的模板，從 template/cpp/main.cpp 複製過來的
ch A.cpp

# 也可以指定主程式的模板，例如產生適合 google code jam 的主程式
ch -m gcj A.cpp

# 如果 A.cpp 存在，在其內插入 template/cpp/number.cpp
ch A.cpp --libs number
```