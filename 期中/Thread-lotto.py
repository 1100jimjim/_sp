import requests
from bs4 import BeautifulSoup
import concurrent.futures
import random
import collections
import numpy as np
import pandas as pd
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from statsmodels.tsa.arima.model import ARIMA
import time  # ✅ 新增 time 模組

# 單一頁面爬蟲 function
def fetch_page_data(page):
    base_url = "https://www.pilio.idv.tw/lto/listbbk.asp?indexpage={}&orderby=new"
    headers = {"User-Agent": "Mozilla/5.0"}

    try:
        url = base_url.format(page)
        response = requests.get(url, headers=headers, timeout=10)
        response.encoding = response.apparent_encoding
        soup = BeautifulSoup(response.text, 'html.parser')
        rows = soup.find_all("tr")

        page_data = []
        for row in rows[1:]:
            cols = row.find_all("td")
            if len(cols) >= 4:
                first_zone_text = cols[2].text.strip()
                second_zone_text = cols[3].text.strip()
                if "," in first_zone_text and second_zone_text.isdigit():
                    first_zone = [int(num) for num in first_zone_text.split(",")]
                    second_zone = int(second_zone_text)
                    page_data.append((first_zone, second_zone))

        return page_data if page_data else None
    except Exception as e:
        print(f"⚠️ 第 {page} 頁抓取失敗: {e}")
        return None

# 使用 thread 加速抓取歷史資料
def fetch_lottery_history():
    history_data = []
    max_threads = 10  # 同時開 10 個 threads
    page = 1

    with concurrent.futures.ThreadPoolExecutor(max_workers=max_threads) as executor:
        while True:
            futures = {executor.submit(fetch_page_data, p): p for p in range(page, page + max_threads)}
            any_valid = False
            for future in concurrent.futures.as_completed(futures):
                result = future.result()
                if result:
                    history_data.extend(result)
                    any_valid = True

            if not any_valid:
                break
            page += max_threads

    return history_data

def extract_features(history_data):
    X, y_first, y_second = [], [], []
    first_zone_freq = collections.Counter()
    second_zone_freq = collections.Counter()

    for i in range(len(history_data) - 1):
        first_zone, second_zone = history_data[i]
        next_first_zone, next_second_zone = history_data[i + 1]

        first_zone_freq.update(first_zone)
        second_zone_freq.update([second_zone])

        features = []
        for num in range(1, 50):
            features.append(first_zone.count(num))
            features.append(first_zone_freq[num])

        features.append(second_zone)
        features.append(second_zone_freq[second_zone])
        features.append(max(first_zone))
        features.append(min(first_zone))
        features.append(np.mean(first_zone))
        features.append(np.std(first_zone))

        X.append(features)
        y_first.append(next_first_zone)
        y_second.append(next_second_zone)

    return np.array(X), np.array(y_first), np.array(y_second)

def train_and_predict(history_data):
    X, y_first, y_second = extract_features(history_data)

    scaler = MinMaxScaler()
    X_scaled = scaler.fit_transform(X)

    X_train, X_test, y_train_first, y_test_first, y_train_second, y_test_second = train_test_split(
        X_scaled, y_first, y_second, test_size=0.2, random_state=42)

    model_first = RandomForestRegressor(n_estimators=300, max_depth=15, random_state=42)
    model_second = RandomForestRegressor(n_estimators=300, max_depth=10, random_state=42)

    model_first.fit(X_train, y_train_first)
    model_second.fit(X_train, y_train_second)

    last_features = X_scaled[-1].reshape(1, -1)
    predicted_first_zone_raw = model_first.predict(last_features).flatten()
    predicted_first_zone = sorted(set(np.round(predicted_first_zone_raw).astype(int)))[:6]
    predicted_second_zone = int(round(model_second.predict(last_features)[0]))

    return predicted_first_zone, predicted_second_zone

def time_series_prediction(history_data):
    first_zone_all = [num for first_zone, _ in history_data for num in first_zone]
    second_zone_all = [second_zone for _, second_zone in history_data]

    model_first = ARIMA(first_zone_all, order=(6, 1, 0)).fit()
    model_second = ARIMA(second_zone_all, order=(3, 1, 1)).fit()

    forecast_first_raw = model_first.forecast(steps=12)
    forecast_first_counts = collections.Counter(np.round(forecast_first_raw).astype(int))
    forecast_first = [num for num, _ in forecast_first_counts.most_common(6)]

    forecast_second = int(round(model_second.forecast(steps=1)[0]))

    return forecast_first, forecast_second

def show_statistics(history_data):
    first_zone_all = [num for first_zone, _ in history_data for num in first_zone]
    second_zone_all = [second_zone for _, second_zone in history_data]

    first_counter = collections.Counter(first_zone_all)
    second_counter = collections.Counter(second_zone_all)

    top_6_first = [num for num, _ in first_counter.most_common(6)]
    top_1_second = second_counter.most_common(1)[0][0]

    print("\n📌 第一區出現最多的 6 個號碼：", top_6_first)
    print("📌 第二區出現最多的 1 個號碼：", top_1_second)

# 主流程
try:
    start_time = time.time()  # ⏱️ 開始時間

    history_data = fetch_lottery_history()

    end_time = time.time()  # ⏱️ 結束時間
    duration = end_time - start_time
    print(f"\n⏱️ 歷史開獎資料爬取完成，耗時：{duration:.2f} 秒")

    print("\n📊 以下為最新的 5 期開獎數據：")
    for i, (first_zone, second_zone) in enumerate(history_data[:5], 1):
        print(f"第 {i} 期: {first_zone} + [{second_zone}]")

    show_statistics(history_data)

    ml_first, ml_second = train_and_predict(history_data)
    ts_first, ts_second = time_series_prediction(history_data)

    print(f"\n🤖 機器學習預測號碼：第一區: {ml_first}，第二區: {ml_second}")
    print(f"📈 時間序列預測號碼：第一區: {ts_first}，第二區: {ts_second}")

except Exception as e:
    print(f"❌ 發生錯誤: {e}")
