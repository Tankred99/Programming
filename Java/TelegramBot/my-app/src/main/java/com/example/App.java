package com.example;

import java.io.IOException;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.Duration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONObject;
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
    private static final String LM_STUDIO_API_URL = "http://localhost:1234/v1/chat/completions";
    private static final int TIMEOUT_SECONDS = 30;

    private Map<Long, String> userModels; // Stores the selected model for each user

    public App() {
        userModels = new HashMap<>();
    }

    public static void main(String[] args) {
        try {
            BOT_TOKEN = readTokenFromFile(TOKENS_FILE_PATH);
            TelegramBotsApi botsApi = new TelegramBotsApi(DefaultBotSession.class);
            botsApi.registerBot(new App());
            printLogHeader("BOT STARTED", "Bot is now running and waiting for messages!");
        } catch (TelegramApiException e) {
            printLogError("BOT STARTUP FAILED", e.getMessage());
            e.printStackTrace();
        } catch (IOException e) {
            printLogError("TOKEN READ ERROR", "Error reading token from file: " + e.getMessage());
            System.exit(1);
        }
    }

    private static String readTokenFromFile(String filePath) throws IOException {
        Path path = Paths.get(filePath);
        List<String> lines = Files.readAllLines(path);

        if (lines.isEmpty()) {
            throw new IOException("tokens.txt file is empty.");
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

            printLog("MESSAGE RECEIVED", "Chat ID: " + chatId, "Content: " + messageText);

            if (messageText.startsWith("/start")) {
                sendStartMessage(chatId);
            } else if (messageText.startsWith("/model")) {
                handleModelSelection(chatId, messageText);
            } else {
                String selectedModel = userModels.getOrDefault(chatId, "codestral-22b-v0.1");
                printLog("MODEL SELECTION", "Chat ID: " + chatId, "Model: " + selectedModel);

                String responseText = sendToLMStudio(messageText, selectedModel);

                SendMessage message = new SendMessage();
                message.setChatId(String.valueOf(chatId));
                message.setText(responseText);

                try {
                    execute(message);
                    printLog("RESPONSE SENT", "Chat ID: " + chatId, "Status: Success");
                } catch (TelegramApiException e) {
                    printLogError("RESPONSE ERROR", "Error sending response to user: " + e.getMessage());
                    e.printStackTrace();
                }
            }
        }
    }

    private void sendStartMessage(long chatId) {
        SendMessage message = new SendMessage();
        message.setChatId(String.valueOf(chatId));
        message.setText("Bot is running and ready to use! Use the /model command to select a model.");

        try {
            execute(message);
            printLog("START MESSAGE", "Chat ID: " + chatId, "Status: Sent");
        } catch (TelegramApiException e) {
            printLogError("START MESSAGE ERROR", "Error sending start message: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void handleModelSelection(long chatId, String messageText) {
        String[] parts = messageText.split(" ");
        if (parts.length == 2) {
            String model = parts[1];
            if (model.equals("codestral-22b-v0.1") || model.equals("mathstral-7b-v0.1")) {
                userModels.put(chatId, model);
                SendMessage message = new SendMessage();
                message.setChatId(String.valueOf(chatId));
                message.setText("You selected model: " + model);

                try {
                    execute(message);
                    printLog("MODEL CHANGED", "Chat ID: " + chatId, "Model: " + model);
                } catch (TelegramApiException e) {
                    printLogError("MODEL SELECTION ERROR", "Error sending confirmation: " + e.getMessage());
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
        message.setText("Please select a model using the command /model codestral-22b-v0.1 or /model mathstral-7b-v0.1.");

        try {
            execute(message);
            printLog("MODEL HELP", "Chat ID: " + chatId, "Status: Sent");
        } catch (TelegramApiException e) {
            printLogError("MODEL HELP ERROR", "Error sending help: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private String sendToLMStudio(String messageText, String model) {
        printLogHeader("LM STUDIO REQUEST", "Starting request to LM Studio API");

        try {
            HttpClient client = HttpClient.newBuilder()
                .connectTimeout(Duration.ofSeconds(TIMEOUT_SECONDS))
                .followRedirects(HttpClient.Redirect.NORMAL)
                .build();

            printLog("HTTP CLIENT", "Status: Created", "Timeout: " + TIMEOUT_SECONDS + "s");

            JSONObject requestJson = new JSONObject();
            requestJson.put("model", model);
            requestJson.put("max_tokens", 40000);
            requestJson.put("stream", false);

            JSONArray messagesArray = new JSONArray();

            JSONObject systemMessage = new JSONObject();
            systemMessage.put("role", "system");
            systemMessage.put("content", "You are a helpful assistant.");
            messagesArray.put(systemMessage);

            JSONObject userMessage = new JSONObject();
            userMessage.put("role", "user");
            userMessage.put("content", messageText);
            messagesArray.put(userMessage);

            requestJson.put("messages", messagesArray);

            String requestBody = requestJson.toString();
            printLog("REQUEST BODY", "Length: " + requestBody.length() + " chars", "Content: " +
                (requestBody.length() > 100 ? requestBody.substring(0, 100) + "..." : requestBody));

            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(LM_STUDIO_API_URL))
                .header("Content-Type", "application/json")
                .header("Accept", "application/json")
                .timeout(Duration.ofSeconds(TIMEOUT_SECONDS))
                .POST(HttpRequest.BodyPublishers.ofString(requestBody))
                .build();

            printLog("REQUEST DETAILS", "Method: " + request.method(), "Headers: " + request.headers().map().toString());

            printLog("REQUEST SENDING", "Status: In Progress", "Waiting for response...");

            try {
                HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
                int statusCode = response.statusCode();

                String responseBody = response.body();
                String logResponseBody = responseBody.length() > 300 ? responseBody.substring(0, 300) + "... [truncated]" : responseBody;

                printLog("RESPONSE RECEIVED", "Status Code: " + statusCode, "Body Length: " + responseBody.length() + " chars");
                printLog("RESPONSE BODY", "Content: " + logResponseBody, "");

                if (statusCode == 200) {
                    JSONObject jsonResponse = new JSONObject(responseBody);
                    String content = jsonResponse.getJSONArray("choices").getJSONObject(0).getJSONObject("message").getString("content");

                    printLog("CONTENT PARSED", "Length: " + content.length() + " chars", "Preview: " +
                        (content.length() > 50 ? content.substring(0, 50) + "..." : content));
                    printLogHeader("LM STUDIO REQUEST", "Request completed successfully");
                    return content;
                } else {
                    String errorMessage = "HTTP " + statusCode + ": " + logResponseBody;
                    printLogError("API ERROR", errorMessage);
                    printLogHeader("LM STUDIO REQUEST", "Request failed with status " + statusCode);
                    return "Sorry, I encountered an error when processing your request. Error code: " + statusCode;
                }
            } catch (IOException e) {
                printLogError("CONNECTION ERROR", "Failed to connect: " + e.getMessage());
                printLog("TROUBLESHOOTING", "Is LM Studio running?", "Check URL: " + LM_STUDIO_API_URL);
                printLogHeader("LM STUDIO REQUEST", "Request failed due to connection error");
                return "Sorry, I couldn't connect to the AI service. Please make sure LM Studio is running on port 1234.";
            }
        } catch (Exception e) {
            printLogError("GENERAL ERROR", "Error with request: " + e.getMessage());
            e.printStackTrace();
            printLogHeader("LM STUDIO REQUEST", "Request failed due to exception");
            return "Sorry, an error occurred while processing your request. Please try again later.";
        }
    }

    // Improved logging methods to create table-like output
    private static void printLogHeader(String type, String message) {
        String border = "+=".repeat(40) + "+";
        System.out.println("\n" + border);
        System.out.println("| " + type + ": " + message);
        System.out.println(border);
    }

    private static void printLog(String type, String detail1, String detail2) {
        String line = "| " + padRight(type, 20) + "| " + padRight(detail1, 30) + "| " + detail2;
        System.out.println(line);
        System.out.println("+--------------------+------------------------------+----------------------------------------+");
    }

    private static void printLogError(String type, String errorMessage) {
        String border = "+".repeat(81);
        System.out.println("\n" + border);
        System.out.println("| ERROR: " + type);
        System.out.println("| " + errorMessage);
        System.out.println(border + "\n");
    }

    private static String padRight(String s, int n) {
        return String.format("%-" + n + "s", s);
    }
}
