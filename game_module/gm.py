import requests

# 请求的目标 URL
url = "http://192.168.4.210:30031/api/oasis/legion/game/210-test7"

# 如果需要发送 JSON 数据
json_data = "{\"infos\":[{\"luid\":1517784820158038272,\"name\":\"LM1\",\"abbr\":\"lm1\",\"serverId\":1,\"happenTime\":1747968396,\"curActUid\":5140494996499079169,\"legions\":[{\"uid\":5122785590328213504,\"sid\":1,\"signIndex\":1,\"matchPower\":765035741,\"winingStreak\":0,\"loseingStreak\":1,\"members\":[{\"puid\":70903889920,\"name\":\"100\",\"power\":263534542,\"castleLv\":23,\"noSoldierPower\":53535819,\"isFormal\":true},{\"puid\":70903922688,\"name\":\"101\",\"power\":261950799,\"castleLv\":25,\"noSoldierPower\":54842312,\"isFormal\":true},{\"puid\":70936756224,\"name\":\"Survivor#10o3zsw\",\"power\":48747058,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70936788992,\"name\":\"Survivor#10o4p34\",\"power\":48747375,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70936985600,\"name\":\"Survivor#10o8wsg\",\"power\":48746950,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937018368,\"name\":\"Survivor#10o9m2o\",\"power\":48744399,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937051136,\"name\":\"Survivor#10oabcw\",\"power\":48745270,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937149440,\"name\":\"Survivor#10ocf7k\",\"power\":48746706,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937214976,\"name\":\"Survivor#10odts0\",\"power\":48739295,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937247744,\"name\":\"Survivor#10oej28\",\"power\":48747401,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937411584,\"name\":\"Survivor#10oi1hc\",\"power\":48743804,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937477120,\"name\":\"Survivor#10ojg1s\",\"power\":48746767,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937608192,\"name\":\"Survivor#10om96o\",\"power\":48747228,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":70937640960,\"name\":\"Survivor#10omygw\",\"power\":48745362,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":71034798080,\"name\":\"Survivor#12ahdds\",\"power\":48745609,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true},{\"puid\":71034896384,\"name\":\"Survivor#12ajh8g\",\"power\":48746249,\"castleLv\":25,\"noSoldierPower\":46904115,\"isFormal\":true}]}]}]}"

# 如果需要添加自定义头部
headers = {
    "Content-Type": "application/json"
}

try:
    # 发送 POST 请求（表单形式）
    response = requests.post(url, data=json_data, headers=headers)

    # 或发送 JSON 数据
    # response = requests.post(url, json=json_data, headers=headers)

    # 检查响应状态
    if response.status_code == 200:
        print("请求成功！")
        print("响应内容：", response.json())  # 如果响应是 JSON
    else:
        print(f"请求失败，状态码：{response.status_code}")
        print("响应内容：", response.text)

except requests.exceptions.RequestException as e:
    print("请求发生错误：", e)

