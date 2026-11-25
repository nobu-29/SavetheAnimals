import json
import joblib

# AI読み込み
model = joblib.load("player_ai.pkl")

# UE5レポート読込
with open(
    "../Saved/PlayerReport.json",
    "r",
    encoding="utf-8"
) as f:
    report = json.load(f)

# AIへ入力
player_data = [[
    report["DeathCount"],
    report["LoadKillCount"],
    report["PoisonCount"],
    report["DiseaseCount"],
    report["RetryCount"],
    report["HintCount"]
]]

# 判定
result = model.predict(player_data)[0]

print(result)

# 結果保存
output = {
    "LearningType": result
}

with open(
    "../Saved/AnalysisResult.json",
    "w",
    encoding="utf-8"
) as f:
    json.dump(
        output,
        f,
        ensure_ascii=False,
        indent=4
    )
