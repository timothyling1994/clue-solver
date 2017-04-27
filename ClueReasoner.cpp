#include "ClueReasoner.h"
using namespace std;

int ClueReasoner::GetPlayerNum(string player)
{
	if (player == case_file)
		return num_players;
	
	for (int i = 0; i < num_players; i++)
		if (player == players[i])
			return i;
			
	cout<<"Illegal player: "<<player<<endl;
	return -1;
}
int ClueReasoner::GetCardNum(string card)
{
	for (int i = 0; i < num_cards; i++)
		if (card == cards[i])
			return i;
			
	cout<<"Illegal card: "<<card<<endl;
	return -1;
}

string ClueReasoner::QueryString(int return_code)
{
	if (return_code == kFalse)
		return "n";
	else if (return_code == kTrue)
		return "Y";
	else
		return "-";
}

void ClueReasoner::PrintNotepad()
{
	for (int i = 0; i < num_players; i++)
		cout<<"\t"<<players[i];
	cout<<"\t"<<case_file<<endl;
	
	for (int i = 0; i < num_cards; i++)
	{
		cout<<cards[i]<<"\t";
		for (int j = 0; j < num_players; j++)
			cout<<QueryString(Query(players[j], cards[i]))<<"\t";
		
		cout<<QueryString(Query(case_file, cards[i]))<<endl;
	}
}
	
void ClueReasoner::AddInitialClauses()
{
	/* The following code is given as an example to show how to create Clauses and post them to the solver. SatSolver.h uses the following typedefs:
		typedef int Literal;
		typedef std::vector<Literal> Clause;
		
	That is, a Literal is defined as a positive or a negative integer (meaning that it is the negated form, as in -p or -q), and a Clause is defined as a vector of Literals (integers).
	
	The function GetPairNum(p, c) returns the literal that means that player p has card c (note that p can be case_file as well). 
	See ClueReasoner.h, lines 7-29 for a definition of the arrays and variables that you can use in your implementation. 
	*/

	// Each card is in at least one place (including case file).
	for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		Clause clause;
		for (int p = 0; p <= num_players; p++)	// Iterate over all players, including the case file (as a possible place for a card).
			clause.push_back(GetPairNum(p, c));
		
		solver->AddClause(clause);
	}
        
	// If a card is in one place, it cannot be in another place.
	//need to show that if it is in this place, then all other spots are negatives
	
	for(int c = 0; c < num_cards; c++)
	{
		for(int p = 0; p <= num_players;p++)
		{
			
			for(int x = 0; x <= num_players; x++)
			{
				if(x != p)
				{
					Clause clause;
					clause.push_back(-1*GetPairNum(p,c));
					clause.push_back(-1*GetPairNum(x,c));
					solver->AddClause(clause);
				}
			}
			
		}
		
	}
	
	
	// At least one card of each category is in the case file.
		
	Clause clause1;
	for(int a = 0; a < 6; a++)
	{
		string suspect = suspects[a];
		clause1.push_back(GetPairNum("cf",suspect));
	}
	solver->AddClause(clause1);
	
	Clause clause2;
	for(int b=0;b<6;b++)
	{
		string weapon = weapons[b];
		clause2.push_back(GetPairNum("cf",weapon));
	}
	solver->AddClause(clause2);
	
	Clause clause3;
	for(int c=0;c<9;c++)
	{
		string room = rooms[c];
		clause3.push_back(GetPairNum("cf",room));
	}
	solver->AddClause(clause3);
	

	// No two cards in each category can both be in the case file.

	for(int a = 0; a < 6; a++)
	{
		for(int b = 0; b < 6; b++)
		{
			Clause clause;
			if(a != b)
			{
				string suspect = suspects[a];
				string suspectb = suspects[b];
				clause.push_back(-1*GetPairNum("cf",suspect));
				clause.push_back(-1*GetPairNum("cf",suspectb));
				solver->AddClause(clause);
			}
		}
	}
	
	for(int a = 0; a < 6; a++)
	{
		for(int b = 0; b < 6; b++)
		{
			Clause clause;
			if(a != b)
			{
				string weapon = weapons[a];
				string weaponb = weapons[b];
				clause.push_back(-1*GetPairNum("cf",weapon));
				clause.push_back(-1*GetPairNum("cf",weaponb));
				solver->AddClause(clause);
			}
		}
	}
	
	for(int a = 0; a < 9; a++)
	{
		for(int b = 0; b < 9; b++)
		{
			Clause clause;
			if(a != b)
			{
				string room = rooms[a];
				string roomb = rooms[b];
				clause.push_back(-1*GetPairNum("cf",room));
				clause.push_back(-1*GetPairNum("cf",roomb));
				solver->AddClause(clause);
			}
		}
	}
	
}
void ClueReasoner::Hand(string player, string cards[3])
{
	// GetPlayerNum returns the index of the player in the players array (not the suspects array). Remember that the players array is sorted wrt the order that the players play. Also note that, player_num (not to be confused with num_players) is a private variable of the ClueReasoner class that is initialized when this function is called.
	//the hand that the player gets cannot be in the case file
	player_num = GetPlayerNum(player);
	for(int i =0; i < 3; i++)
	{
		Clause clause;
		Clause clause1; 
		clause.push_back(GetPairNum(player,cards[i]));
		solver->AddClause(clause);
	}
	
	// TO BE IMPLEMENTED AS AN EXERCISE
}
void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
	
	// Note that in the Java implementation, the refuter and the card_shown can be NULL. 
	// In this C++ implementation, NULL is translated to be the empty string "".
	// To check if refuter is NULL or card_shown is NULL, you should use if(refuter == "") or if(card_shown == ""), respectively.
	
	
	if(refuter == "") //no one refutes it, either suggester or the in case file
	{
		Clause clause; 
		clause.push_back(GetPairNum(suggester,card1));
		clause.push_back(GetPairNum("cf",card1));
		solver->AddClause(clause);
		
		Clause clause1; 
		clause1.push_back(GetPairNum(suggester,card2));
		clause1.push_back(GetPairNum("cf",card2));
		solver->AddClause(clause1);
		
		Clause clause2; 
		clause2.push_back(GetPairNum(suggester,card3));
		clause2.push_back(GetPairNum("cf",card3));
		solver->AddClause(clause2);
		
		
	}
	else // there is a refuter
	{
		if (card_shown == "") //there is a refuter but no card shown
		{
			Clause clause;
			clause.push_back(GetPairNum(refuter,card1));
			clause.push_back(GetPairNum(refuter,card2));
			clause.push_back(GetPairNum(refuter,card3));
			solver->AddClause(clause);
			
			
			///////////////////cr.Suggest("sc", "sc", "ro", "lo", "mu", "sc");
			
			
			bool go = true;
			
			for (int i = GetPlayerNum(suggester)+1; i < 6; i++)
			{
				if(go)
				{
					if(players[i] != refuter)
					{
						Clause clause1;
						Clause clause2;
						Clause clause3;
						clause1.push_back(-1*GetPairNum(players[i],card1));
						clause2.push_back(-1*GetPairNum(players[i],card2));
						clause3.push_back(-1*GetPairNum(players[i],card3));		
						solver->AddClause(clause1);
						solver->AddClause(clause2);	
						solver->AddClause(clause3);
					}
					else
					{
						go = false;
					}
				}
			}
					
					
					for(int i = 0; i < GetPlayerNum(suggester); i++)
					{
						if(go)
						{
							if(players[i] != refuter)
							{
								Clause clause1;
								Clause clause2;
								Clause clause3;
								clause1.push_back(-1*GetPairNum(players[i],card1));
								clause2.push_back(-1*GetPairNum(players[i],card2));
								clause3.push_back(-1*GetPairNum(players[i],card3));		
								solver->AddClause(clause1);
								solver->AddClause(clause2);	
								solver->AddClause(clause3);
							}
							else
							{
								go = false;
							}
						}
					}
		}
		else //card is shown
		{
			Clause shown_clause;
			shown_clause.push_back(GetPairNum(refuter,card_shown));
			solver->AddClause(shown_clause);
						
						
						///////////////////cr.Suggest("sc", "sc", "ro", "lo", "mu", "sc");
						
						
						bool go = true;
						
						for (int i = GetPlayerNum(suggester)+1; i < 6; i++)
						{
							if(go)
							{
								if(players[i] != refuter)
								{
									Clause clause1;
									Clause clause2;
									Clause clause3;
									clause1.push_back(-1*GetPairNum(players[i],card1));
									clause2.push_back(-1*GetPairNum(players[i],card2));
									clause3.push_back(-1*GetPairNum(players[i],card3));		
									solver->AddClause(clause1);
									solver->AddClause(clause2);	
									solver->AddClause(clause3);
								}
								else
								{
									go = false;
								}
							}
						}
								
								
								for(int i = 0; i < GetPlayerNum(suggester); i++)
								{
									if(go)
									{
										if(players[i] != refuter)
										{
											Clause clause1;
											Clause clause2;
											Clause clause3;
											clause1.push_back(-1*GetPairNum(players[i],card1));
											clause2.push_back(-1*GetPairNum(players[i],card2));
											clause3.push_back(-1*GetPairNum(players[i],card3));		
											solver->AddClause(clause1);
											solver->AddClause(clause2);	
											solver->AddClause(clause3);
										}
										else
										{
											go = false;
										}
									}
								}
							
								
		}
	}
		
	
	
	//need to account for the clockwise rotation 

}
void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	
	if(is_correct)
	{
		Clause clause;
		Clause clause1;
		Clause clause2;
		clause.push_back(GetPairNum("cf",card1));
		clause1.push_back(GetPairNum("cf",card2));
		clause2.push_back(GetPairNum("cf",card3));
		solver->AddClause(clause);
		solver->AddClause(clause1);
		solver->AddClause(clause2);
	}
	else
	{
		Clause clause;
		Clause clause1;
		Clause clause2;
		clause.push_back(-1*GetPairNum("cf",card1));
		clause1.push_back(-1*GetPairNum("cf",card2));
		clause2.push_back(-1*GetPairNum("cf",card3));
		solver->AddClause(clause);
		solver->AddClause(clause1);
		solver->AddClause(clause2);
	}
	
}
