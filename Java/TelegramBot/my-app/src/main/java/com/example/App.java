package com.example;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
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
    private static final String BOT_USERNAME = "AI Zed Bot";
    private static final String TOKENS_FILE_PATH = ".env/tokens.txt";
    private static final String LM_STUDIO_API_URL =
        "http://localhost:1234/v1/chat/completions"; // Замените на URL вашего LM Studio API

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
            System.exit(1);
        }
    }

    private static String readTokenFromFile(String filePath)
        throws IOException {
        Path path = Paths.get(filePath);
        List<String> lines = Files.readAllLines(path);

        if (lines.isEmpty()) {
            throw new IOException("Файл tokens.txt пуст.");
        }

        return lines.get(0).trim();
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

            // Отправляем запрос к LM Studio
            String responseText = sendToLMStudio(messageText);

            SendMessage message = new SendMessage();
            message.setChatId(String.valueOf(chatId));
            message.setText(responseText);

            try {
                execute(message);
            } catch (TelegramApiException e) {
                e.printStackTrace();
            }
        }
    }

    private String sendToLMStudio(String prompt) {
        try {
            HttpClient client = HttpClient.newHttpClient();
            String requestBody = String.format(
                "{\"prompt\": \"%s\", \"max_tokens\": 200000}",
                prompt
            );

            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(LM_STUDIO_API_URL))
                .header("Content-Type", "application/json")
                .POST(HttpRequest.BodyPublishers.ofString(requestBody))
                .build();

            HttpResponse<String> response = client.send(
                request,
                HttpResponse.BodyHandlers.ofString()
            );

            if (response.statusCode() == 200) {
                return response.body();
            } else {
                return (
                    "Ошибка при запросе к LM Studio: " + response.statusCode()
                );
            }
        } catch (Exception e) {
            e.printStackTrace();
            return "Ошибка при запросе к LM Studio: " + e.getMessage();
        }
    }
}
