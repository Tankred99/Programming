package com.example;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import org.telegram.telegrambots.bots.TelegramLongPollingBot;
import org.telegram.telegrambots.meta.TelegramBotsApi;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;
import org.telegram.telegrambots.meta.api.objects.Update;
import org.telegram.telegrambots.meta.exceptions.TelegramApiException;
import org.telegram.telegrambots.updatesreceivers.DefaultBotSession;

public class App extends TelegramLongPollingBot {

    private static String BOT_TOKEN;
    private static final String BOT_USERNAME = "YOUR_BOT_USERNAME"; // Замените на имя пользователя вашего бота
    private static final String TOKENS_FILE_PATH = ".env/tokens.txt";

    public static void main(String[] args) {
        try {
            BOT_TOKEN = readTokenFromFile(TOKENS_FILE_PATH);
            TelegramBotsApi botsApi = new TelegramBotsApi(
                DefaultBotSession.class
            );
            botsApi.registerBot(new App());
        } catch (TelegramApiException e) {
            e.printStackTrace();
        } catch (IOException e) {
            System.err.println(
                "Ошибка при чтении токена из файла: " + e.getMessage()
            );
            System.exit(1); // Завершаем программу с кодом ошибки
        }
    }

    // Метод для чтения токена из файла
    private static String readTokenFromFile(String filePath)
        throws IOException {
        Path path = Paths.get(filePath);
        List<String> lines = Files.readAllLines(path);

        if (lines.isEmpty()) {
            throw new IOException("Файл tokens.txt пуст.");
        }

        return lines.get(0).trim(); // Берем первую строку как токен и удаляем лишние пробелы
    }

    @Override
    public String getBotUsername() {
        return BOT_USERNAME;
    }

    @Override
    public String getBotToken() {
        return BOT_TOKEN;
    }

    @Override
    public void onUpdateReceived(Update update) {
        if (update.hasMessage() && update.getMessage().hasText()) {
            String messageText = update.getMessage().getText();
            long chatId = update.getMessage().getChatId();

            SendMessage message = new SendMessage();
            message.setChatId(String.valueOf(chatId));
            message.setText("Вы сказали: " + messageText);

            try {
                execute(message);
            } catch (TelegramApiException e) {
                e.printStackTrace();
            }
        }
    }
}
