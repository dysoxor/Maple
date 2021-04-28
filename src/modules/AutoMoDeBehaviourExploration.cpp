/**
  * @file <src/modules/AutoMoDeBehaviourExploration.cpp>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#include "AutoMoDeBehaviourExploration.h"


namespace argos {

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourExploration::AutoMoDeBehaviourExploration() {
		m_strLabel = "Exploration";
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourExploration::AutoMoDeBehaviourExploration(AutoMoDeBehaviourExploration* pc_behaviour) {
		m_strLabel = pc_behaviour->GetLabel();
		m_bLocked = pc_behaviour->IsLocked();
		m_bOperational = pc_behaviour->IsOperational();
		m_unIndex = pc_behaviour->GetIndex();
		m_unIdentifier = pc_behaviour->GetIdentifier();
		m_mapParameters = pc_behaviour->GetParameters();
		Init();
	}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourExploration::~AutoMoDeBehaviourExploration() {}

	/****************************************/
	/****************************************/

	AutoMoDeBehaviourExploration* AutoMoDeBehaviourExploration::Clone() {
		return new AutoMoDeBehaviourExploration(this);
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourExploration::Init() {
		m_unTurnSteps = 0;
		m_eExplorationState = RANDOM_WALK;
		m_fProximityThreshold = 0.25;
		m_bLocked = false;
		std::map<std::string, Real>::iterator it = m_mapParameters.find("rwm");
		if (it != m_mapParameters.end()) {
			m_cRandomStepsRange.SetMax(it->second);
		} else {
			LOGERR << "[FATAL] Missing parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}

		// Success probability
		it = m_mapParameters.find("p");
		if (it != m_mapParameters.end()) {
			m_fSuccessProbabilityParameter = it->second;
		} else {
			LOGERR << "[FATAL] Missing probability parameter for the following behaviour:" << m_strLabel << std::endl;
			THROW_ARGOSEXCEPTION("Missing Parameter");
		}
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourExploration::ControlStep() {
		switch (m_eExplorationState) {
			case RANDOM_WALK: {
				m_pcRobotDAO->SetWheelsVelocity(m_pcRobotDAO->GetMaxVelocity(), m_pcRobotDAO->GetMaxVelocity());
				if (ObstacleInFront()) {
					m_eExplorationState = OBSTACLE_AVOIDANCE;
					m_unTurnSteps = (m_pcRobotDAO->GetRandomNumberGenerator())->Uniform(m_cRandomStepsRange);
					CRadians cAngle = m_pcRobotDAO->GetProximityReading().Angle;
					if (cAngle.GetValue() < 0) {
						m_eTurnDirection = LEFT;
					} else {
						m_eTurnDirection = RIGHT;
					}
				}
				break;
			}
			case OBSTACLE_AVOIDANCE: {
				m_unTurnSteps -= 1;
				switch (m_eTurnDirection) {
					case LEFT: {
						m_pcRobotDAO->SetWheelsVelocity(-m_pcRobotDAO->GetMaxVelocity(), m_pcRobotDAO->GetMaxVelocity());
						break;
					}
					case RIGHT: {
						m_pcRobotDAO->SetWheelsVelocity(m_pcRobotDAO->GetMaxVelocity(), -m_pcRobotDAO->GetMaxVelocity());
						break;
					}
				}
				if (m_unTurnSteps <= 0) {
					m_eExplorationState = RANDOM_WALK;
				}
				break;
			}
		}
		m_bLocked = false;
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourExploration::Reset() {
		m_bOperational = false;
		Init();
		ResumeStep();
	}

	/****************************************/
	/****************************************/

	void AutoMoDeBehaviourExploration::ResumeStep() {
		m_bOperational = true;
	}

	/****************************************/
	/****************************************/

	bool AutoMoDeBehaviourExploration::Succeeded() {
		return EvaluateBernoulliProbability(m_fSuccessProbabilityParameter);
	}

	/****************************************/
	/****************************************/

	bool AutoMoDeBehaviourExploration::Failed() {
		return false;
	}
}
