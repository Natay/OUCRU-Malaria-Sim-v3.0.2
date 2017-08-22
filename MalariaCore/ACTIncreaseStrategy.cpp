/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ACTIncreaseStrategy.cpp
 * Author: Merlin
 * 
 * Created on April 26, 2017, 11:20 AM
 */

#include "ACTIncreaseStrategy.h"
#include "Random.h"
#include "Model.h"
#include <assert.h>

ACTIncreaseStrategy::ACTIncreaseStrategy() {
}

ACTIncreaseStrategy::ACTIncreaseStrategy(const ACTIncreaseStrategy& orig) {
}

ACTIncreaseStrategy::~ACTIncreaseStrategy() {
}

std::vector<Therapy*>& ACTIncreaseStrategy::get_therapy_list(){
    return therapy_list_;
}

void ACTIncreaseStrategy::add_therapy(Therapy* therapy) {
    therapy_list_.push_back(therapy);
}

bool ACTIncreaseStrategy::is_strategy(const std::string& sName) {
    return ("ACTIncreaseStrategy" == sName);
}

Therapy* ACTIncreaseStrategy::get_therapy() {
    double P = Model::RANDOM->random_flat(0.0, 1.0);

    double sum = 0;
    for (int i = 0; i < distribution_.size(); i++) {
        sum += distribution_[i];
        if (P <= sum) {
            return therapy_list()[i];
        }
    }

    return therapy_list()[therapy_list().size() - 1];
}

std::string ACTIncreaseStrategy::to_string() const {
    return "ACTIncreaseStrategy";
}

IStrategy::StrategyType ACTIncreaseStrategy::get_type() const {
    return IStrategy::MFT;
}

void ACTIncreaseStrategy::update_end_of_time_step() {   
    if (Model::SCHEDULER->current_time() % 30 == 0) {
        //TODO: test here
        adjustDisttribution(Model::SCHEDULER->current_time(), Model::SCHEDULER->total_time());
    }
}

void ACTIncreaseStrategy::adjustDisttribution(int time, int totaltime) {

    double dACT = ((end_distribution_[0] - start_distribution_[0]) * time) / totaltime + start_distribution_[0];

    distribution_[0] = dACT;
    double otherD = (1 - dACT) / (distribution_.size() - 1);
    for (int i = 1; i < distribution_.size(); i++) {
        distribution_[i] = otherD;
    }
}