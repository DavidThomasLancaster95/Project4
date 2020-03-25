#ifndef DATABASE_H
#define DATABASE_H

#include "DatalogProgram.h"
#include <vector>
#include "Relation.h"
#include <string>
#include "SchemeVec.h"
#include "ConstantMaker.h"

class DataBase
{
private:

	std::vector<Relation> RelationsVector;
	
	DatalogProgram rawData;

	std::vector<Relation> QueryResultsVector;

public:

	DataBase(DatalogProgram InRawData) {
		//std::cout << "STARTED!\n";
		rawData = InRawData;
		buildRelationsVector();

		processRules();

		processQueries();
		//printAllQueryResults();
		projectRemove();
		printAndProcess();

		

		//std::cout << "FINISHED!\n";
	}

	void processRules() {

		// Pseudocode

		// Loop through every rule until it adds no new tuples


		//Relation relation0 = RelationsVector.at(0);
		//Relation relation1 = RelationsVector.at(1);

		//Relation relation2 = relation0.joinRelation(relation1);

		//relation2.printRelation();

		std::cout << "Rule Evaluation\n";

		bool tuplesMade = true; 
		bool hasBeenMarked = false; 

		int countNumber = 0; 

		while (tuplesMade || hasBeenMarked) {
			hasBeenMarked = false; 
			for (unsigned int i = 0; i < rawData.getRuleVector().size(); i++) {
				Predicate headpredicate = rawData.getRuleVector().at(i).getHeadPredicate();
				std::vector<Predicate> predicateList = rawData.getRuleVector().at(i).getPredicateList();
				// Add into ruleRelationsVector
				std::vector<Relation> ruleRelationsVector;
				Relation joinedRelationTotal;

				// go through each predicate
				for (unsigned int k = 0; k < predicateList.size(); k++) {
					Predicate thePredicate = predicateList.at(k);
					ruleRelationsVector.push_back(getRelationFromPredicate(thePredicate));
				}

				// join them together
				joinedRelationTotal = ruleRelationsVector.at(0);
				if (ruleRelationsVector.size() == 1) {

				}
				else {
					for (unsigned int k = 1; k < ruleRelationsVector.size(); k++) {
						joinedRelationTotal = joinedRelationTotal.joinRelation(ruleRelationsVector.at(k));
					}
				}

				// project only needed parts
				std::vector<std::string> removeStrings;

				for (int k = 0; k < joinedRelationTotal.getSchemeVector().size(); k++) {
					if (!vectorContains(headpredicate.returnElements(), joinedRelationTotal.getSchemeVector().at(k))) {
						removeStrings.push_back(joinedRelationTotal.getSchemeVector().at(k));
						// remove time

					}
				}
				for (unsigned int k = 0; k < removeStrings.size(); k++) {
					joinedRelationTotal.ProjectByString(removeStrings.at(k));
				}

				// Reorder to match head>>>>>>>>>>>>>

				for (unsigned int k = 0; k < headpredicate.returnElements().size(); k++) {
					if (joinedRelationTotal.getSchemeVector().at(k) != headpredicate.returnElements().at(k)) {
						// reorder joinRelaitonTotal.
						joinedRelationTotal.reorder(headpredicate.returnElements().at(k), k);
					}
				}

				// print all the crap you need - header and relations
					// header

				printRuleFromRule(rawData.getRuleVector().at(i));

				// relations

				// ----->joinedRelationTotal.PrintRelationTuples();


				// checking if tuples are being made

				//------>std::cout << joinedRelationTotal.getSetOfTupleVecs().size() << std::endl; 

				int before;
				for (unsigned int k = 0; k < RelationsVector.size(); k++) {
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {
						//----->std::cout << "BEFORE: " << RelationsVector.at(k).getSetOfTupleVecs().size() << std::endl;
						before = RelationsVector.at(k).getSetOfTupleVecs().size();
					}
				}

				// do a union with relational database. ^^^*|*^^^

				for (unsigned int k = 0; k < RelationsVector.size(); k++) {
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {
						
						// Actually, the printing is just done in this function here. Forget what it says around....
						printAdditions(RelationsVector.at(k), joinedRelationTotal);
						RelationsVector.at(k).unite(joinedRelationTotal);
					
					}
				}

				 

				// check size difference
				int after;
				for (unsigned int k = 0; k < RelationsVector.size(); k++) {
					if (RelationsVector.at(k).getNameOfRelation() == headpredicate.returnID()) {
						//------>std::cout << "After: " << RelationsVector.at(k).getSetOfTupleVecs().size() << std::endl;
						after = RelationsVector.at(k).getSetOfTupleVecs().size();
					}
				}

				if (before == after) {
					tuplesMade = false;
				}
				else {
					tuplesMade = true;
					hasBeenMarked = true; 
				}
				//std::cout << "1 Pass Completed" << std::endl; 


			}
			

			countNumber++;

		}

		std::cout << "\nSchemes populated after " << countNumber << " passes through the Rules.\n\n";

		

			// Follow procedure for rule. 

				// 1. evaluate predicates - get them as Relations

				// 2. join them together*

				// 3. Project to only include columns in the head predicate*

				// 4. Reorder to match the head predicate.*

				// 5. Print that relation*

				

	}

	void buildRelationsVector() {
		// For every element of SchemesPredicate vector...
		for (unsigned int i = 0; i < rawData.getSchemesPredicateVector().size(); i++) {

			// CREATE A NEW RELATION
			Relation relation;

			// ADD A NEW NAME
			relation.setNameOfRelation(rawData.getSchemesPredicateVector().at(i).returnID());

			// ADD every Element of the Scheme Vector
			for (unsigned int k = 0; k < rawData.getSchemesPredicateVector().at(i).returnElements().size(); k++) {
				
				relation.addToSchemeVector(rawData.getSchemesPredicateVector().at(i).returnElements().at(k));
				
			}

			// make a tuple vector
			for (unsigned int k = 0; k < rawData.getFactsPredicateVector().size(); k++) {
				TupleVec myTupleVec;
				
				if (rawData.getFactsPredicateVector().at(k).returnID() == relation.getNameOfRelation()) {
					for (unsigned int j = 0; j < rawData.getFactsPredicateVector().at(k).returnElements().size(); j++) {
						myTupleVec.pushBack(rawData.getFactsPredicateVector().at(k).returnElements().at(j));
						
					}
					relation.addTupleVectorToSet(myTupleVec);

				}
			}
			RelationsVector.push_back(relation);
		}
	}

	void processQueries() {
		std::cout << "Query Evaluation\n";

		for (unsigned int i = 0; i < rawData.getQueriesPredicateVector().size(); i++) {
			processSingleQuery(rawData.getQueriesPredicateVector().at(i));
		}
	}

	void processSingleQuery(Predicate inPredicate) {
		
		for (unsigned int i = 0; i < RelationsVector.size(); i++) {


			if (RelationsVector.at(i).getNameOfRelation() == inPredicate.returnID()) { // match the name
				Relation returnRelation = RelationsVector.at(i);
				// for each parameter...
				
 				for (unsigned int k = 0; k < inPredicate.returnElements().size(); k++) {
					if (/*Its a string */isAString(inPredicate.returnElements().at(k))) {
						returnRelation = returnRelation.SelectString(inPredicate.returnElements().at(k), k);
					}
					else {
						// it its a consonant
						
						ConstantMaker constantMaker(inPredicate.returnElements().at(k));
							// Find and add the values
						//constantMaker.pushPositionVector(k);
						for (unsigned int j = k; j < inPredicate.returnElements().size(); j++) {
							if (inPredicate.returnElements().at(j) == inPredicate.returnElements().at(k)) {
								constantMaker.pushPositionVector(j);
							}
						}

						for (unsigned int x = 0; x < constantMaker.getPositionVector().size(); x++) {
							//std::cout << constantMaker.getPositionVector().at(x) << " ";
						}

						//std::cout << "-n";
						// select for the constant marker. 
						returnRelation = returnRelation.SelectConstant(constantMaker);
					}
				}
				//int x = 1; // what is the return relation value????
				QueryResultsVector.push_back(returnRelation);
			}
		}
	}

	Relation getRelationFromPredicate(Predicate inPredicate) {
		// Loop through the relations vector
		for (unsigned int i = 0; i < RelationsVector.size(); i++) {
			//once the name matches the inPredicate returnId
			if (RelationsVector.at(i).getNameOfRelation() == inPredicate.returnID()) {
				// Create a new Relation
				Relation returnRelation = RelationsVector.at(i);
				// Go through the elements of the inPredicate
				for (unsigned int k = 0; k < inPredicate.returnElements().size(); k++) {
					// This code gets the relation.... I think. 
					if (/*Its a string */isAString(inPredicate.returnElements().at(k))) {
						returnRelation = returnRelation.SelectString(inPredicate.returnElements().at(k), k);
					}
					else {
						// it its a consonant

						ConstantMaker constantMaker(inPredicate.returnElements().at(k));
						// Find and add the values
					//constantMaker.pushPositionVector(k);
						for (unsigned int j = k; j < inPredicate.returnElements().size(); j++) {
							if (inPredicate.returnElements().at(j) == inPredicate.returnElements().at(k)) {
								constantMaker.pushPositionVector(j);
							}
						}

						for (unsigned int x = 0; x < constantMaker.getPositionVector().size(); x++) {
							//std::cout << constantMaker.getPositionVector().at(x) << " ";
						}

						//std::cout << "-n";
						// select for the constant marker. 
						returnRelation = returnRelation.SelectConstant(constantMaker);
					}
				}
				return returnRelation;
			}
		}

		Relation bsRelation;
		return bsRelation;

	}

	void projectRemove() {
		// Rename the appropriate schemes
		
		//printAllQueryResults();
		// remove the rows.
		for (int i = QueryResultsVector.size() - 1; i >= 0 ; i--) { // try iterating backwards on this one too original code: unsigned int i = 0; i < QueryResultsVector.size(); i++
			for (int k = QueryResultsVector.at(i).getSchemeVector().size() - 1; k >= 0 ; k--) { // Try iterating backwards original code: unsigned int k = 0; k < QueryResultsVector.at(i).getSchemeVector().size(); k++
				
				std::string asdf = rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				if (isAString(asdf)) {
					//std::cout << QueryResultsVector.at(i).getSchemeVector().at(k) << "\n";

					QueryResultsVector.at(i).ProjectByString(QueryResultsVector.at(i).getSchemeVector().at(k));
					
				}
				else {
					//removedDuplicateConstants();
				}
			}

			//std::cout << "\n";
		}

		//printAllQueryResults();

		removedDuplicateConstants();

	}
	
	void removedDuplicateConstants() {
		for (int i = QueryResultsVector.size() - 1; i >= 0; i--) { 
			for (int k = QueryResultsVector.at(i).getSchemeVector().size() - 1; k >= 0; k--) { 
				std::string value = QueryResultsVector.at(i).getSchemeVector().at(k);  //rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				if (!isAString(value)) {
					std::string currentValue = value;
					for (int j = k - 1; j >= 0; j--) {
						std::string value2 = QueryResultsVector.at(i).getSchemeVector().at(j);
						if (value2 == currentValue) {

							//QueryResultsVector.at(i).ProjectByString(currentValue);

							QueryResultsVector.at(i).ProjectByIndex(k);
						}
					}
				}
			}
		}
	}

	void printAndProcess() {
		for (unsigned int i = 0; i < QueryResultsVector.size(); i++) {
			std::cout << rawData.getQueriesPredicateVector().at(i).returnID();
			std::cout << "(";
			for (unsigned int k = 0; k < rawData.getQueriesPredicateVector().at(i).returnElements().size(); k++) {
				if (k == rawData.getQueriesPredicateVector().at(i).returnElements().size() - 1) {
					std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k);
				}
				else {
					std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << ",";
				}
			}
			std::cout << ")? ";

			// Yes/no (x) 

			if (QueryResultsVector.at(i).getSetOfTupleVecs().size() > 0) {
				std::cout << "Yes(" << QueryResultsVector.at(i).getSetOfTupleVecs().size() << ")";
			}
			else {
				std::cout << "No";
			}

			std::cout << "\n";

			


			//for (unsigned int k = 0; k < rawData.getQueriesPredicateVector().at(i).returnElements().size(); k++) {
			//	
			//	std::cout << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << "\n";
			//	if (!isAString(rawData.getQueriesPredicateVector().at(i).returnElements().at(k))) {
			//		// Project that element by string. 
			//		//QueryResultsVector.at(i).
			//		std::cout << "___" << rawData.getQueriesPredicateVector().at(i).returnElements().at(k) << "___\n";
			//		//std::cout << "---" << QueryResultsVector.at(i).getSetOfTupleVecs(). << "---\n";
			//		QueryResultsVector.at(i).ProjectByString(rawData.getQueriesPredicateVector().at(i).returnElements().at(k));
			//	}
			//}

			//QueryResultsVector.at(i).printRelation();
			QueryResultsVector.at(i).PrintRelationTuples();
		}

	}
	


	// -----------------Helper Functions---------------------

	bool isAString(std::string inString) {
		char firstChar = inString.at(0);
		if (firstChar == '\'') {
			return true;
		}
		else {
			return false;
		}
	}

	void printAllQueryResults() {
		std::cout << "\n\n----PRINTING QUERY TESTS----\n\n";
		

		for (unsigned int i = 0; i < QueryResultsVector.size(); i++) {
			QueryResultsVector.at(i).printRelation();


			QueryResultsVector.at(i).PrintRelationTuples();
			std::cout << "\n\n";
		}
	}

	bool vectorContains(std::vector<std::string> inVector, std::string inSymbol) {
		for (unsigned int i = 0; i < inVector.size(); i++) {
			if (inVector.at(i) == inSymbol) {
				return true;
			}
		}
		return false;
	}

	void printRuleFromRule(Rule inRule) {
		//std::cout << "   ";
		std::cout << inRule.getHeadPredicate().returnID();
		std::cout << "(";
		for (unsigned int i = 0; i < inRule.getHeadPredicate().returnElements().size(); i++) {
			if (i == 0) {
				std::cout << inRule.getHeadPredicate().returnElements().at(i);
			}
			else {
				std::cout << "," << inRule.getHeadPredicate().returnElements().at(i);
			}
		}
		std::cout << ") :- ";
		for (unsigned int i = 0; i < inRule.getPredicateList().size(); i++) {
			std::cout << inRule.getPredicateList().at(i).returnID();
			std::cout << "(";
			for (unsigned int k = 0; k < inRule.getPredicateList().at(i).returnElements().size(); k++) {
				if (k == 0) {
					std::cout << inRule.getPredicateList().at(i).returnElements().at(k);
				}
				else {
					std::cout << "," << inRule.getPredicateList().at(i).returnElements().at(k);
				}
			}
			
			if (i == inRule.getPredicateList().size() - 1) {
				std::cout << ").";
			}
			else {
				std::cout << "),";
			}
		}
		std::cout << "\n";
	}

	void printAdditions(Relation mainRelation, Relation joinedRelationTotal) {
		

		Relation uniqueAdditions;

		Relation originalRelation = mainRelation;

		mainRelation.unite(joinedRelationTotal);
		
		std::set<TupleVec> tupVecSet = mainRelation.getSetOfTupleVecs();
		std::set<TupleVec> tupVecSet2 = originalRelation.getSetOfTupleVecs();

		for (auto it = tupVecSet.begin(); it != tupVecSet.end(); ++it) {
			TupleVec testVec = *it;

			bool hasit = false;
			for (auto it2 = tupVecSet2.begin(); it2 != tupVecSet2.end(); ++it2) {
				TupleVec testVec2 = *it2; 

				if (testVec2.getVector() == testVec.getVector()) {
					hasit = true;
				}
			}

			if (!hasit) {
				uniqueAdditions.addTupleVectorToSet(testVec);
			}
		}

		uniqueAdditions.setSchemeVector(originalRelation.getSchemeVector());
		uniqueAdditions.setNameOfRelation(originalRelation.getNameOfRelation());
		uniqueAdditions.PrintRelationTuples();

	}

	

};

#endif