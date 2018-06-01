斗地主平台

服务器

# 大厅 Hall
getAllRooms()


# 房间 Room
status -> [play, recruit]

game -> Game

members -> User[]

add(userId) -> 添加房间

# 游戏 Game
status -> [rub, play]

member -> User[]

landlord -> User

start():

rubLandlord(score): 叫地主：1 2 3 分

onStatusChanged(status, prevStatus): 状态改变，开始游戏什么的

onDeal(cards): 发牌事件

onPlayingCard(user, cards): 玩家出牌事件

# 用户 User
cards -> Card[]

login(username): 登录

playCard(cards): 出牌

