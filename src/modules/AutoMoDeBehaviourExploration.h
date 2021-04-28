/**
  * @file <src/modules/AutoMoDeBehaviourExploration.h>
  *
  * @author Antoine Ligot - <aligot@ulb.ac.be>
  *
  * @package ARGoS3-AutoMoDe
  *
  * @license MIT License
  */

#ifndef AUTOMODE_BEHAVIOUR_EXPLORATION_H
#define AUTOMODE_BEHAVIOUR_EXPLORATION_H

#include "AutoMoDeBehaviour.h"

namespace argos {
	class AutoMoDeBehaviourExploration: public AutoMoDeBehaviour {
		public:
			AutoMoDeBehaviourExploration();
			AutoMoDeBehaviourExploration(AutoMoDeBehaviourExploration* pc_behaviour);
			virtual ~AutoMoDeBehaviourExploration();

			virtual void ControlStep();
			virtual void Reset();
			virtual void ResumeStep();
			virtual void Init();

			virtual AutoMoDeBehaviourExploration* Clone();

			virtual bool Succeeded();
			virtual bool Failed();

		private:
			SInt32 m_unTurnSteps;

			enum ExplorationState {
				RANDOM_WALK,
				OBSTACLE_AVOIDANCE
			};

			enum TurnDirection {
				LEFT,
				RIGHT
			};

			ExplorationState m_eExplorationState;
			TurnDirection m_eTurnDirection;

			Real m_fProximityThreshold;
			CRange<UInt32> m_cRandomStepsRange;
	};
}

#endif
