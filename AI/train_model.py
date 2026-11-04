import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import joblib

# CSV読込
df = pd.read_csv("training_data.csv")

# 特徴量
X = df[
    [
        "DeathCount",
        "LoadKillCount",
        "PoisonCount",
        "DiseaseCount",
        "RetryCount"
    ]
]

# 正解ラベル
y = df["Type"]

# AI作成
model = DecisionTreeClassifier()

# 学習
model.fit(X, y)

# 保存
joblib.dump(model, "player_ai.pkl")

print("学習完了")