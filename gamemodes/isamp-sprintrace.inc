#if defined _isamp_sprintrace_included
	#endinput
#endif
#define _isamp_sprintrace_included

//=================================DEFINES======================================

//==================================NEWS========================================

new /* Sistema de picadas */
	SprintRaceOffer[MAX_PLAYERS],
	SprintRaceOpponent[MAX_PLAYERS],
	SprintRaceBet[MAX_PLAYERS],
	SprintRaceCountdownSecs[MAX_PLAYERS],
	SprintRaceTimer[MAX_PLAYERS],
	SprintRaceCancelOfferTimer[MAX_PLAYERS];

/* Sistema de picadas - Posiciones de llegada */
new Float:SPRINT_RACE_POS[27][3] = {
    {1846.5231,-1366.7672,12.9625},
	{1820.0952,-1828.6703,12.9792},
	{1682.6759,-2163.8623,17.0619},
	{1530.0300,-1734.9821,12.9484},
	{1366.6411,-1397.9254,12.9656},
	{1020.7272,-1141.5991,23.2214},
	{1301.4172,-927.4117,39.0356},
	{898.4853,-761.1699,97.4632},
	{270.0749,-1115.5090,78.7906},
	{94.2488,-1514.7880,6.0249},
	{520.7662,-1260.1526,15.6716},
	{484.5140,-1501.8690,19.9247},
	{784.0887,-1676.9086,12.8284},
	{1079.4280,-2342.4229,11.9586},
	{1722.7888,-2687.0054,5.4470},
	{2391.5979,-2665.8567,13.0817},
	{2763.1636,-2458.6868,13.0951},
	{2836.0371,-1969.0796,10.5005},
	{2781.3474,-1045.8680,36.6520},
	{1599.3545,-1342.5320,29.6871},
	{2314.8547,-1661.9110,13.7055},
	{2592.1982,-1561.0543,8.4551},
	{2581.3362,-2180.7651,-0.6541},
	{1923.7242,-1840.4707,3.5495},
	{1617.0505,-1642.9298,13.0261},
	{1170.7081,-1570.0011,12.8763},
	{367.8909,-2038.0934,7.2342}
};

//=============================RESET VARIABLES==================================


resetSprintRace(playerid)
{
	SprintRaceOffer[playerid] = 999;
	SprintRaceOpponent[playerid] = 999;
	SprintRaceBet[playerid] = 0;
	SprintRaceCountdownSecs[playerid] = 11;
}

deleteAbandonedSprintRace(playerid)
{
	/* Sistema de picadas */
	new winnerid = SprintRaceOpponent[playerid];
	if(winnerid != 999)
	{
	    if(SprintRaceOpponent[winnerid] == playerid)
	    {
	        new finishRaceMessage[128];
			SendClientMessage(winnerid, COLOR_YELLOW2, "Tu contricante ha abandonado ((Desconectado)) la carrera, ganas la carrera.");
			GivePlayerCash(winnerid, SprintRaceBet[winnerid]*2);
			format(finishRaceMessage, sizeof(finishRaceMessage), "El organizador te entrega tus $%d de la carrera!", SprintRaceBet[winnerid]*2);
		    SendClientMessage(playerid, COLOR_WHITE, finishRaceMessage);
		    SprintRaceOpponent[playerid] = 999;
		    SprintRaceOpponent[winnerid] = 999;
		    SprintRaceBet[winnerid] = 0;
		    SprintRaceBet[playerid] = 0;
		    DisablePlayerRaceCheckpoint(winnerid);
		}
	}
	/* Fin sistema de picadas */
}

deleteFinishedSprintRace(playerid)
{
    /* Sistema de picadas */
	new racerid = SprintRaceOpponent[playerid];
    if(racerid != 999)
	{
	    if(SprintRaceOpponent[racerid] == playerid)
	    {
		    if(racerid != INVALID_PLAYER_ID)
		    {
		        if(SprintRaceBet[racerid] == SprintRaceBet[playerid])
		        {
		            new finishRaceMessage[128];
		            new ganadorid = playerid;
		            GameTextForPlayer(ganadorid, "Has ganado la carrera!", 1000, 3);
		            GameTextForPlayer(racerid, "Has perdido la carrera!", 1000, 3);
		            DisablePlayerRaceCheckpoint(playerid);
		            DisablePlayerRaceCheckpoint(racerid);
		    		GivePlayerCash(playerid, SprintRaceBet[racerid]*2);
			    	format(finishRaceMessage, sizeof(finishRaceMessage), "El organizador te entrega tus $%d de la carrera!", SprintRaceBet[racerid]*2);
				    SendClientMessage(playerid, COLOR_WHITE, finishRaceMessage);
				    format(finishRaceMessage, sizeof(finishRaceMessage), "El organizador se queda con tus $%d de la carrera y se lo reparte al ganador!", SprintRaceBet[racerid]);
				    SendClientMessage(racerid, COLOR_WHITE, finishRaceMessage);
				    SprintRaceOpponent[playerid] = 999;
		 			SprintRaceOpponent[racerid] = 999;
				    SprintRaceBet[playerid] = 0;
				    SprintRaceBet[racerid] = 0;
				}
			}
		}
	}
	/* Fin sistema de picadas */
}

//==============================SISTEMA DE PICADAS==============================

startSprintRaceChallenge(playerid)
{
	new challengerid = SprintRaceOffer[playerid];
    if(SprintRaceOpponent[playerid] == 999)
    {
   		if(challengerid != 999 && challengerid != INVALID_PLAYER_ID)
   		{
     		if(IsPlayerInRangeOfPoint(playerid, 20.0, 2337.5864,-1245.0068,22.5000))
    		{
     			if(SprintRaceOpponent[playerid] == 999 && SprintRaceOpponent[challengerid] == 999)
			 	{
			 	    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerState(challengerid) == PLAYER_STATE_DRIVER)
			 	    {
			 	        new moneyBet = SprintRaceBet[playerid];
						if(GetPlayerCash(playerid) > moneyBet && GetPlayerCash(challengerid) > moneyBet)
						{
     						if(ProxDetectorS(10.0, playerid, challengerid))
     						{
     							KillTimer(SprintRaceCancelOfferTimer[playerid]);
								new content[128];
							   	format(content, sizeof(content), "ha aceptado el desafio de %s a una carrera sprint por $%d.", GetPlayerNameEx(challengerid), moneyBet);
								PlayerActionMessage(playerid, 15.0, content);
								SprintRaceOpponent[playerid] = challengerid;
								SprintRaceOpponent[challengerid] = playerid;
								SprintRaceOffer[playerid] = 999;
								SprintRaceBet[challengerid] = moneyBet;
								TogglePlayerControllable(playerid, false);
								TogglePlayerControllable(challengerid, false);
								SendClientMessage(playerid, COLOR_WHITE, "Le entregas al organizador la suma acordada");
								SendClientMessage(challengerid, COLOR_WHITE, "Le entregas al organizador la suma acordada");
								GivePlayerCash(playerid, -moneyBet);
								GivePlayerCash(challengerid, -moneyBet);
								new randomPos = random (27);
								SetPlayerRaceCheckpoint(playerid, 1, SPRINT_RACE_POS[randomPos][0],SPRINT_RACE_POS[randomPos][1],SPRINT_RACE_POS[randomPos][2], 0.0, 0.0, 0.0, 4.0);
							    SetPlayerRaceCheckpoint(challengerid, 1, SPRINT_RACE_POS[randomPos][0],SPRINT_RACE_POS[randomPos][1],SPRINT_RACE_POS[randomPos][2], 0.0, 0.0, 0.0, 4.0);
								SendClientMessage(playerid, COLOR_YELLOW2, "La carrera comienza en 10 segundos. Se ha marcado la ubicacion de la meta en tu GPS. Preparate!");
								SendClientMessage(challengerid, COLOR_YELLOW2, "La carrera comienza en 10 segundos. Se ha marcado la ubicacion de la meta en tu GPS. Preparate!");
								SprintRaceTimer[challengerid] = SetTimerEx("SprintRaceCountdown", 1000, 1, "ii", challengerid, playerid);
     						} else
     						    {
     						    	SprintRaceCancel(playerid);
	   								SendClientMessage(playerid, COLOR_WHITE, "Debes estar cerca del otro corredor.");
								}
						} else
						    {
						    	SprintRaceCancel(playerid);
	    						SendClientMessage(playerid, COLOR_WHITE, "Alguno de los dos no tiene esa cantidad de efectivo para apostar!");
							}
			 	    } else
			 	        {
			 	            SprintRaceCancel(playerid);
	   						SendClientMessage(playerid, COLOR_WHITE, "Ambos deben estar de conductor en el auto con el cual correr�n!");
						}
				} else
				    {
						SendClientMessage(playerid, COLOR_WHITE, "Alguno de los dos est� en una carrera!");
					}
	   		} else
		    	{
		    	    SprintRaceCancel(playerid);
	    			SendClientMessage(playerid, COLOR_WHITE, "Para aceptar un desafio, debes estar en el punto de reunion de corredores conocido como 'El S�tano'");
				}
	    } else
	    	{
	    	    SprintRaceCancel(playerid);
				SendClientMessage(playerid, COLOR_WHITE, "Nadie te ha desafiado a una carrera de sprint / Jugador inv�lido.");
			}
    } else
        {
    		SendClientMessage(playerid, COLOR_WHITE, "Ya est�s en una carrera!");
		}
}

//---------------------------SISTEMA DE PICADAS---------------------------------
//---------------------------SISTEMA DE PICADAS---------------------------------

stock SprintRaceCancel(playerid)
{
    SprintRaceOffer[playerid] = 999;
	SprintRaceBet[playerid] = 0;
	KillTimer(SprintRaceCancelOfferTimer[playerid]);
}

forward SprintRaceCancelOffer(playerid);
public SprintRaceCancelOffer(playerid)
{
    SprintRaceOffer[playerid] = 999;
	SprintRaceBet[playerid] = 0;
	return 1;
}

forward SprintRaceCountdown(racerid, racerid2);
public SprintRaceCountdown(racerid, racerid2)
{
    SprintRaceCountdownSecs[racerid] --;
	if (SprintRaceCountdownSecs[racerid] > 0)
	{
 		new sprintRaceCountdownString[8];
		format(sprintRaceCountdownString, sizeof(sprintRaceCountdownString), "%d", SprintRaceCountdownSecs[racerid]);
 		PlayerPlaySound(racerid, 1056, 0.0, 0.0, 0.0);
 		PlayerPlaySound(racerid2, 1056, 0.0, 0.0, 0.0);
		GameTextForPlayer(racerid, sprintRaceCountdownString, 500, 3);
		GameTextForPlayer(racerid2, sprintRaceCountdownString, 500, 3);
	}
	if(SprintRaceCountdownSecs[racerid] == 0)
	{
		GameTextForPlayer(racerid, "YA!!!", 500, 3);
		GameTextForPlayer(racerid2, "YA!!!", 500, 3);
		PlayerPlaySound(racerid, 1057, 0.0, 0.0, 0.0);
		PlayerPlaySound(racerid2, 1057, 0.0, 0.0, 0.0);
		TogglePlayerControllable(racerid, true);
		TogglePlayerControllable(racerid2, true);
		SprintRaceCountdownSecs[racerid] = 11;
		KillTimer(SprintRaceTimer[racerid]);
	}
	return 1;
}

CMD:desafiarpicada(playerid, params[])
{
	new racerid, moneyBet;

	if(sscanf(params, "ii", racerid, moneyBet))
		return SendClientMessage(playerid, COLOR_LIGHTYELLOW2, "{5CCAF1}[Sintaxis]:{C8C8C8} /desafiarpicada [ID-Jugador] [dinero]");

    if(!IsPlayerInRangeOfPoint(playerid, 20.0, 2337.5864,-1245.0068,22.5000))
    	return SendClientMessage(playerid, COLOR_WHITE, "Para desafiar a alguien, debes ir al punto de encuentro de corredores conocido como 'El Sotano'.");

 	if(racerid == INVALID_PLAYER_ID || racerid == playerid)
 	    return SendClientMessage(playerid, COLOR_WHITE, "Jugador invalido.");

	if(SprintRaceOpponent[playerid] != 999 || SprintRaceOpponent [racerid] != 999)
	    return SendClientMessage(playerid, COLOR_WHITE, "Alguno de los dos est� en una carrera!");

	if(SprintRaceOffer[playerid] != 999)
 	    return SendClientMessage(playerid, COLOR_WHITE, "Ya tienes un desafio pendiente!");

	if(SprintRaceOffer[racerid] != 999)
 	    return SendClientMessage(playerid, COLOR_WHITE, "El sujeto esta con otro desafio en este instante, aguarda.");

	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerState(racerid) != PLAYER_STATE_DRIVER)
	    return SendClientMessage(playerid, COLOR_WHITE, "Ambos deben estar de conductor en el auto con el cual correr�n!");

	if(moneyBet < 500)
 		return SendClientMessage(playerid, COLOR_WHITE, "La apuesta m�nima es de $500.");

	if(GetPlayerCash(playerid) < moneyBet)
 		return SendClientMessage(playerid, COLOR_WHITE, "No tienes esa cantidad de dinero para apostar!");

	if(!ProxDetectorS(10.0, playerid, racerid))
  	    return SendClientMessage(playerid, COLOR_WHITE, "El jugador que desafiaste no est� cerca tuyo.");

	new content[128];
   	format(content, sizeof(content), "%s te ha desafiado a una carrera por $%d. La oferta termina en 15 segundos. Usa /aceptar picada si lo deseas.", GetPlayerNameEx(playerid), moneyBet);
	SendClientMessage(racerid, COLOR_LIGHTBLUE, content);
	format(content, sizeof(content), "Has desafiado a %s a una carrera por $%d. La oferta termina en 15 segundos. Espera su respuesta.", GetPlayerNameEx(racerid), moneyBet);
	SendClientMessage(playerid, COLOR_LIGHTBLUE, content);
	SprintRaceOffer[racerid] = playerid;
	SprintRaceBet[racerid] = moneyBet;
	SprintRaceCancelOfferTimer[racerid] = SetTimerEx("SprintRaceCancelOffer", 15000, false, "i", racerid);
	return 1;
}