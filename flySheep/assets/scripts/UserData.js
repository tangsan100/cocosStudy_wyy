

var UserData = cc.Class({
    extends: cc.Component,

    properties: {
       
    },

    ctor:function(){
        // 取出本地数据

        var str = cc.sys.localStorage.getItem('userScore');
        
        if (str && str != 'undefined'){
           
            userScore = JSON.parse(str);
            userScore.highest = userScore.highest ? userScore.highest:0;
            userScore.score = 0;
            return;
        }

        
        
    },

    addScore:function(){
        if (!userScore.score) userScore.score = 0;
        if (!userScore.highest) userScore.highest = 0;

        userScore.score += 1;
        userScore.highest = userScore.highest > userScore.score? userScore.highest : userScore.score;

        cc.log(userScore);

        cc.sys.localStorage.setItem('userScore',JSON.stringify(userScore));
    },

});
