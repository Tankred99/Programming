package com.example;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
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
        "http://localhost:1234/v1/chat/completions";

    private Map<Long, String> userModels; // Хранит выбранную модель для каждого пользователя

    public App() {
        userModels = new HashMap<>();
    }

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

            if (messageText.startsWith("/model")) {
                handleModelSelection(chatId, messageText);
            } else {
                String selectedModel = userModels.getOrDefault(
                    chatId,
                    "mathstral"
                );
                String responseText = sendToLMStudio(
                    messageText,
                    selectedModel
                );

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
    }

    private void handleModelSelection(long chatId, String messageText) {
        String[] parts = messageText.split(" ");
        if (parts.length == 2) {
            String model = parts[1];
            if (model.equals("mathstral") || model.equals("codestral")) {
                userModels.put(chatId, model);
                SendMessage message = new SendMessage();
                message.setChatId(String.valueOf(chatId));
                message.setText("Вы выбрали модель: " + model);

                try {
                    execute(message);
                } catch (TelegramApiException e) {
                    e.printStackTrace();
                }
            } else {
                sendModelSelectionHelp(chatId);
            }
        } else {
            sendModelSelectionHelp(chatId);
        }
    }

    private void sendModelSelectionHelp(long chatId) {
        SendMessage message = new SendMessage();
        message.setChatId(String.valueOf(chatId));
        message.setText(
            "Пожалуйста, выберите модель, используя команду /model mathstral или /model codestral."
        );

        try {
            execute(message);
        } catch (TelegramApiException e) {
            e.printStackTrace();
        }
    }

    private String sendToLMStudio(String prompt, String model) {
        try {
            HttpClient client = HttpClient.newHttpClient();
            String requestBody = String.format(
                "{\"prompt\": \"%s\", \"model\": \"%s\", \"max_tokens\": 150}",
                prompt,
                model
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
