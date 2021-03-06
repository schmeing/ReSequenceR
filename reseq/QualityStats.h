#ifndef QUALITYSTATS_H
#define QUALITYSTATS_H

#include <array>
#include <stdint.h>
#include <vector>

#include "SeqQualityStats.hpp"
#include "utilities.hpp"
#include "Vect.hpp"

namespace reseq{
	class QualityStats{
	public:
		static const uintSeqLen kSqFragmentLengthBinSize = 10;
		static const bool kWriteOut4dMatrixCsvs = false;

	private:
		// kWriteOut3dMatrixCsvs variables
		std::vector<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>> tmp_csv_seg0_a_base_quality_preceding_quality_sequence_quality_position_;
		std::vector<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>> tmp_csv_seg1_c_base_quality_preceding_quality_error_rate_position_;

		// Temporary variables
		std::array<std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 4>, 2> tmp_base_quality_stats_per_tile_per_error_reference_;
		std::array<std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 4>, 2> tmp_error_rate_for_position_per_tile_per_error_reference_;
		std::array<std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 4>, 2> tmp_base_quality_for_error_rate_per_tile_per_error_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_base_quality_for_preceding_quality_per_tile_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_preceding_quality_for_error_rate_per_tile_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_preceding_quality_for_position_per_tile_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_base_quality_for_sequence_quality_per_tile_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_preceding_quality_for_sequence_quality_per_tile_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_sequence_quality_for_error_rate_per_tile_reference_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 4>, 2> tmp_sequence_quality_for_position_per_tile_reference_;

		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_sequence_quality_mean_for_gc_per_tile_reference_;
		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_sequence_quality_mean_for_mean_error_rate_per_tile_reference_;
		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_sequence_quality_mean_for_fragment_length_per_tile_reference_;
		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_mean_error_rate_for_gc_per_tile_reference_;
		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_mean_error_rate_for_fragment_length_per_tile_reference_;
		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_gc_for_fragment_length_per_tile_reference_;

		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 2> tmp_base_quality_for_sequence_per_tile_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 2> tmp_base_quality_for_preceding_quality_per_tile_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 2> tmp_base_quality_stats_per_tile_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 2> tmp_preceding_quality_for_sequence_per_tile_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 2> tmp_preceding_quality_for_position_per_tile_;
		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>>, 5>, 2> tmp_sequence_quality_for_position_per_tile_;

		std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>, 2> tmp_base_quality_stats_per_strand_;

		std::array<std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 5>, 2> tmp_sequence_quality_for_base_per_tile_;
		std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>> tmp_sequence_quality_mean_paired_per_tile_;
		std::array<std::vector<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>>, 2> tmp_sequence_quality_mean_for_gc_per_tile_;
		std::array<std::vector<utilities::VectorAtomic<uintFragCount>>, 2> tmp_sequence_quality_probability_mean_;
		std::array<std::vector<utilities::VectorAtomic<uintFragCount>>, 2> tmp_sequence_quality_minimum_;
		std::array<std::vector<utilities::VectorAtomic<uintFragCount>>, 2> tmp_sequence_quality_first_quartile_;
		std::array<std::vector<utilities::VectorAtomic<uintFragCount>>, 2> tmp_sequence_quality_median_;
		std::array<std::vector<utilities::VectorAtomic<uintFragCount>>, 2> tmp_sequence_quality_third_quartile_;
		std::array<std::vector<utilities::VectorAtomic<uintFragCount>>, 2> tmp_sequence_quality_maximum_;
		std::array<std::vector<std::vector<utilities::VectorAtomic<uintFragCount>>>, 2> tmp_sequence_quality_content_;

		std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>> tmp_homoquality_distribution_;
		std::array<std::array<std::vector<utilities::VectorAtomic<uintNucCount>>, 5>, 2> tmp_nucleotide_quality_;

		// Collected variables for estimation (based on reference)
		std::array<std::array<std::array<Vect<Vect<SeqQualityStats<uintNucCount>>>, 5>, 4>, 2> base_quality_stats_per_tile_per_error_reference_; // base_quality_stats_per_tile_per_error_reference_[first/second][refBase][domError][tileId][readPosition][quality] = #reads
		std::array<std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 4>, 2> error_rate_for_position_per_tile_per_error_reference_; // error_rate_for_position_per_tile_per_error_reference_[first/second][refBase][domError][tileId][position][errorRate];
		std::array<std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 4>, 2> base_quality_for_error_rate_per_tile_per_error_reference_; // base_quality_for_error_rate_per_tile_per_error_reference_[first/second][refBase][domError][tileId][errorRate][baseQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> base_quality_for_preceding_quality_per_tile_reference_; // base_quality_for_preceding_quality_per_tile_reference_[first/second][refBase][tileId][qualityOfPrecedingBase][baseQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> preceding_quality_for_error_rate_per_tile_reference_; // preceding_quality_for_error_rate_per_tile_reference_[first/second][currentRefBase][tileId][errorRate][precedingQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> preceding_quality_for_position_per_tile_reference_; // preceding_quality_for_position_per_tile_reference_[first/second][currentRefBase][tileId][currentPosition][precedingQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> base_quality_for_sequence_quality_per_tile_reference_; // base_quality_for_sequence_quality_per_tile_reference_[first/second][refBase][tileId][sequenceQuality][baseQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> preceding_quality_for_sequence_quality_per_tile_reference_; // preceding_quality_for_sequence_quality_per_tile_reference_[first/second][refBase][tileId][sequenceQuality][qualityOfPrecedingBase] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> sequence_quality_for_error_rate_per_tile_reference_; // sequence_quality_for_error_rate_per_tile_reference_[first/second][currentRefBase][tileId][errorRate][sequenceQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> sequence_quality_for_position_per_tile_reference_; // sequence_quality_for_position_per_tile_reference_[first/second][currentRefBase][tileId][currentPosition][sequenceQuality] = #bases

		std::array<size_t, 100> threshold_sum_; // Precalculated values required for MeanErrorRateCorrectionIndex
		std::array<Vect<Vect<SeqQualityStats<uintFragCount>>>, 2> sequence_quality_mean_for_gc_per_tile_reference_; // sequence_quality_mean_for_gc_per_tile_reference_[first/second][tileId][percentageGC][qualityMean] = #reads
		std::array<Vect<Vect<Vect<uintFragCount>>>, 2> sequence_quality_mean_for_mean_error_rate_per_tile_reference_; // sequence_quality_mean_for_mean_error_rate_per_tile_reference_[first/second][tileId][meanErrorRate][qualityMean] = #reads
		std::array<Vect<Vect<Vect<uintFragCount>>>, 2> sequence_quality_mean_for_fragment_length_per_tile_reference_; // sequence_quality_mean_for_fragment_length_per_tile_reference_[first/second][tileId][fragmentLength][qualityMean] = #reads
		std::array<Vect<Vect<Vect<uintFragCount>>>, 2> mean_error_rate_for_gc_per_tile_reference_; // mean_error_rate_for_gc_per_tile_reference_[first/second][tileId][percentageGC][meanErrorRate] = #reads
		std::array<Vect<Vect<Vect<uintFragCount>>>, 2> mean_error_rate_for_fragment_length_per_tile_reference_; // mean_error_rate_for_fragment_length_per_tile_reference_[first/second][tileId][fragmentLength][meanErrorRate] = #reads
		std::array<Vect<Vect<Vect<uintFragCount>>>, 2> gc_for_fragment_length_per_tile_reference_; // gc_for_fragment_length_per_tile_reference_[first/second][tileId][fragmentLength][percentageGC] = #reads

		// Collected variables for plotting (based on raw reads)
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 2> base_quality_for_sequence_per_tile_; // base_quality_for_sequence_per_tile_[first/second][base][tileId][qualityProbabilityMeanOfSequence][baseQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 2> base_quality_for_preceding_quality_per_tile_; // base_quality_for_preceding_quality_per_tile_[first/second][base][tileId][qualityOfPrecedingBase][baseQuality] = #bases
		std::array<std::array<Vect<Vect<SeqQualityStats<uintNucCount>>>, 5>, 2> base_quality_stats_per_tile_; // base_quality_stats_per_tile_[first/second][base][tileId][readPosition][quality] = #reads
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 2> preceding_quality_for_sequence_per_tile_; // preceding_quality_for_sequence_per_tile_[first/second][currentBase][tileId][qualityProbabilityMeanOfSequence][precedingQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 2> preceding_quality_for_position_per_tile_; // preceding_quality_for_position_per_tile_[first/second][currentBase][tileId][currentPosition][precedingQuality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 5>, 2> sequence_quality_for_position_per_tile_; // sequence_quality_for_position_per_tile_[first/second][base][tileId][position][sequenceQualityProbabilityMean];

		std::array<Vect<SeqQualityStats<uintNucCount>>, 2> base_quality_stats_per_strand_; // base_quality_stats_per_strand_[+/-][readPosition][quality] = #bases

		std::array<std::array<Vect<Vect<SeqQualityStats<uintFragCount>>>, 5>, 2> sequence_quality_for_base_per_tile_; // sequence_quality_for_base_per_tile_[first/second][nucleotide][tileId][percentageNucleotide][qualityProbabilityMean] = #reads
		Vect<Vect<Vect<uintFragCount>>> sequence_quality_mean_paired_per_tile_; // sequence_quality_mean_paired_per_tile_[tileId][qualityMeanFirst][qualityMeanSecond] = #reads
		std::array<Vect<Vect<SeqQualityStats<uintFragCount>>>, 2> sequence_quality_mean_for_gc_per_tile_; // sequence_quality_mean_for_gc_per_tile_[first/second][tileId][percentageGC][qualityMean] = #reads
		std::array<Vect<uintFragCount>, 2> sequence_quality_probability_mean_; // sequence_quality_probability_mean_[first/second][errorProbabilityMeanAsQuality] = #reads
		std::array<Vect<uintFragCount>, 2> sequence_quality_minimum_; // sequence_quality_minimum_[first/second][qualityMinimum] = #reads
		std::array<Vect<uintFragCount>, 2> sequence_quality_first_quartile_; // sequence_quality_first_quartile_[first/second][qualitySecondQuartile] = #reads
		std::array<Vect<uintFragCount>, 2> sequence_quality_median_; // sequence_quality_median_[first/second][qualityMedian] = #reads
		std::array<Vect<uintFragCount>, 2> sequence_quality_third_quartile_; // sequence_quality_third_quartile_[first/second][qualityThirdQuartile] = #reads
		std::array<Vect<uintFragCount>, 2> sequence_quality_maximum_; // sequence_quality_maximum_[first/second][qualityMaximum] = #reads
		std::array<Vect<Vect<uintFragCount>>, 2> sequence_quality_content_; // sequence_quality_content_[first/second][quality][#qualityOccurences] = #reads

		Vect<Vect<uintNucCount>> homoquality_distribution_; // homoquality_distribution_[quality][length] = #homoqualities
		std::array<std::array<SeqQualityStats<uintNucCount>, 5>, 2> nucleotide_quality_; // nucleotide_quality_[first/second][A/C/G/T/N][quality] = #bases

		// Calculated variables for estimation (based on reference)
		std::array<std::array<Vect<Vect<SeqQualityStats<uintNucCount>>>, 4>, 2> base_quality_stats_per_tile_reference_; // base_quality_stats_per_tile_reference_[first/second][refBase][tileId][readPosition][quality] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> error_rate_for_position_per_tile_reference_; // error_rate_for_position_per_tile_reference_[first/second][refBase][tileId][position][errorRate] = #bases
		std::array<std::array<Vect<Vect<Vect<uintNucCount>>>, 4>, 2> base_quality_for_error_rate_per_tile_reference_; // base_quality_for_error_rate_per_tile_reference_[first/second][refBase][tileId][errorRate][baseQuality] = #bases

		// Calculated variables for plotting from variables for estimation (based on reference)
		std::array<Vect<SeqQualityStats<uintNucCount>>, 2> base_quality_stats_reference_; // base_quality_stats_reference_[first/second][readPosition][quality] = #bases
		std::array<Vect<double>, 2> base_quality_mean_reference_; // base_quality_mean_reference_[first/second][readPosition] = baseQualityMean
		std::array<Vect<uintQual>, 2> base_quality_minimum_reference_; // base_quality_minimum_reference_[first/second][readPosition] = baseQualityMinimum
		std::array<Vect<uintQual>, 2> base_quality_first_quartile_reference_; // base_quality_first_quartile_reference_[first/second][readPosition] = baseQualityFirstQuartile
		std::array<Vect<uintQual>, 2> base_quality_median_reference_; // base_quality_median_reference_[first/second][readPosition] = baseQualityMedian
		std::array<Vect<uintQual>, 2> base_quality_third_quartile_reference_; // base_quality_third_quartile_reference_[first/second][readPosition] = baseQualityThirdQuartile
		std::array<Vect<uintQual>, 2> base_quality_maximum_reference_; // base_quality_maximum_reference_[first/second][readPosition] = baseQualityMaximum

		std::array<Vect<uintQual>, 2> average_sequence_quality_for_gc_; // average_sequence_quality_for_gc_[first/second][percentageGC] = averageSequenceQualityProbabilityMean

		// Calculated variables for plotting (based on raw reads)
		std::array<Vect<SeqQualityStats<uintNucCount>>, 2> base_quality_stats_; // base_quality_stats_[first/second][readPosition][quality] = #bases
		std::array<Vect<double>, 2> base_quality_mean_; // base_quality_mean_[first/second][readPosition] = baseQualityMean
		std::array<Vect<uintQual>, 2> base_quality_minimum_; // base_quality_minimum_[first/second][readPosition] = baseQualityMinimum
		std::array<Vect<uintQual>, 2> base_quality_first_quartile_; // base_quality_first_quartile_[first/second][readPosition] = baseQualityFirstQuartile
		std::array<Vect<uintQual>, 2> base_quality_median_; // base_quality_median_[first/second][readPosition] = baseQualityMedian
		std::array<Vect<uintQual>, 2> base_quality_third_quartile_; // base_quality_third_quartile_[first/second][readPosition] = baseQualityThirdQuartile
		std::array<Vect<uintQual>, 2> base_quality_maximum_; // base_quality_maximum_[first/second][readPosition] = baseQualityMaximum
		std::array<Vect<Vect<intQualDiff>>, 2> tile_quality_mean_difference_; //tile_quality_mean_difference_[first/second][tile][readPosition] = meanDifferenceToTotalQualityMean
		std::array<Vect<double>, 2> base_quality_mean_per_strand_; // base_quality_mean_per_strand_[+/-][readPosition] = baseQualityMean

		std::array<Vect<Vect<uintNucCount>>, 2> base_quality_for_sequence_; // base_quality_for_sequence_[first/second][qualityProbabilityMeanOfSequence][baseQuality] = #bases
		std::array<Vect<Vect<uintNucCount>>, 2> base_quality_for_preceding_quality_; // base_quality_for_preceding_quality_[first/second][qualityOfPrecedingBase][baseQuality] = #bases

		std::array<Vect<uintFragCount>, 2> sequence_quality_mean_; // sequence_quality_mean_[first/second][qualityMean] = #reads
		std::array<Vect<Vect<uintFragCount>>, 2> sequence_quality_mean_per_tile_; // sequence_quality_mean_per_tile_[first/second][tileId][qualityMean] = #reads
		Vect<Vect<uintFragCount>> sequence_quality_mean_paired_; // sequence_quality_mean_paired_[qualityMeanFirst][qualityMeanSecond] = #reads
		std::array<Vect<uintQual>, 2> mean_sequence_quality_mean_by_fragment_length_; // mean_sequence_quality_mean_by_fragment_length_[first/second][fragmentLength] = meanQualityMean
		std::array<std::array<Vect<uintQual>, 5>, 2> average_sequence_quality_for_base_; // average_sequence_quality_for_base_[first/second][nucleotide][percentageNucleotide] = averageSequenceQualityMean

		// Helper functions
		size_t MeanErrorRateCorrectionIndex(uintPercent mean_error_rate, uintPercent rate_threshold, uintPercent rate_reduction){
			// First 101 are normal mean error rate
			return ((static_cast<size_t>(rate_threshold)-1)*rate_threshold/2 + rate_reduction-1)*100 - threshold_sum_[rate_threshold] - (static_cast<size_t>(rate_reduction)-1)*(rate_reduction-2)/2 + mean_error_rate - rate_reduction + 101;
		}
		void SplitPairedSequenceQuality();
		void SumTiles();
		void CalculateQualityStats();
		
		// Boost archive functions
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int UNUSED(version)){
			ar & base_quality_stats_per_tile_per_error_reference_;
			ar & error_rate_for_position_per_tile_per_error_reference_;
			ar & base_quality_for_error_rate_per_tile_per_error_reference_;
			ar & base_quality_for_preceding_quality_per_tile_reference_;
			ar & preceding_quality_for_error_rate_per_tile_reference_;
			ar & preceding_quality_for_position_per_tile_reference_;
			ar & base_quality_for_sequence_quality_per_tile_reference_;
			ar & preceding_quality_for_sequence_quality_per_tile_reference_;
			ar & sequence_quality_for_error_rate_per_tile_reference_;
			ar & sequence_quality_for_position_per_tile_reference_;

			ar & sequence_quality_mean_for_gc_per_tile_reference_;
			ar & sequence_quality_mean_for_mean_error_rate_per_tile_reference_;
			ar & sequence_quality_mean_for_fragment_length_per_tile_reference_;
			ar & mean_error_rate_for_gc_per_tile_reference_;
			ar & mean_error_rate_for_fragment_length_per_tile_reference_;
			ar & gc_for_fragment_length_per_tile_reference_;

			ar & base_quality_for_sequence_per_tile_;
			ar & base_quality_for_preceding_quality_per_tile_;
			ar & base_quality_stats_per_tile_;
			ar & preceding_quality_for_sequence_per_tile_;
			ar & preceding_quality_for_position_per_tile_;
			ar & sequence_quality_for_position_per_tile_;

			ar & base_quality_stats_per_strand_;

			ar & sequence_quality_for_base_per_tile_;
			ar & sequence_quality_mean_paired_per_tile_;
			ar & sequence_quality_mean_for_gc_per_tile_;
			ar & sequence_quality_probability_mean_;
			ar & sequence_quality_minimum_;
			ar & sequence_quality_first_quartile_;
			ar & sequence_quality_median_;
			ar & sequence_quality_third_quartile_;
			ar & sequence_quality_maximum_;
			ar & sequence_quality_content_;

			ar & homoquality_distribution_;
			ar & nucleotide_quality_;
		}

		// Google test
		friend class QualityStatsTest;
		friend class ProbabilityEstimatesTest;

	public:
		QualityStats(){
			for( uintTempSeq template_segment=2; template_segment--; ){
				// There are no quality values below 2 for Illumina
				sequence_quality_mean_paired_per_tile_.SetOffset(2);
				sequence_quality_probability_mean_.at(template_segment).SetOffset(2);
				sequence_quality_minimum_.at(template_segment).SetOffset(2);
				sequence_quality_first_quartile_.at(template_segment).SetOffset(2);
				sequence_quality_median_.at(template_segment).SetOffset(2);
				sequence_quality_third_quartile_.at(template_segment).SetOffset(2);
				sequence_quality_maximum_.at(template_segment).SetOffset(2);
				sequence_quality_content_.at(template_segment).SetOffset(2);

				for(auto called_base = 5; called_base--; ){
					nucleotide_quality_.at(template_segment).at(called_base).SetOffset(2);
				}
			}
		}

		// Getter functions (tile_id might not exist if the parameter combination before does not exist in the data (like ref_base, dom_error combination), so use empty dummy if that is the case)
		inline const Vect<SeqQualityStats<uintNucCount>> &BaseQualityStatsReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall ref_base, uintBaseCall dom_error) const{
			return base_quality_stats_per_tile_per_error_reference_.at(template_segment).at(ref_base).at(dom_error)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &ErrorRateForPositionReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base, uintBaseCall dom_error) const{
			return error_rate_for_position_per_tile_per_error_reference_.at(template_segment).at(base).at(dom_error)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &BaseQualityForErrorRateReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base, uintBaseCall dom_error) const{
			return base_quality_for_error_rate_per_tile_per_error_reference_.at(template_segment).at(base).at(dom_error)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &BaseQualityForPrecedingQualityReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return base_quality_for_preceding_quality_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &PrecedingQualityForErrorRateReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return preceding_quality_for_error_rate_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &PrecedingQualityForPositionReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return preceding_quality_for_position_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &BaseQualityForSequenceQualityReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return base_quality_for_sequence_quality_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &PrecedingQualityForSequenceQualityReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return preceding_quality_for_sequence_quality_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &SequenceQualityForErrorRateReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return sequence_quality_for_error_rate_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &SequenceQualityForPositionReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return sequence_quality_for_position_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}

		inline const Vect<SeqQualityStats<uintFragCount>> &SequenceQualityMeanForGCPerTileReference(uintTempSeq template_segment, uintTileId tile_id) const{
			return sequence_quality_mean_for_gc_per_tile_reference_.at(template_segment)[tile_id];
		}
		inline const Vect<Vect<uintFragCount>> &SequenceQualityMeanForMeanErrorRatePerTileReference(uintTempSeq template_segment, uintTileId tile_id) const{
			return sequence_quality_mean_for_mean_error_rate_per_tile_reference_.at(template_segment)[tile_id];
		}
		inline const Vect<Vect<uintFragCount>> &SequenceQualityMeanForFragmentLengthPerTileReference(uintTempSeq template_segment, uintTileId tile_id) const{
			return sequence_quality_mean_for_fragment_length_per_tile_reference_.at(template_segment)[tile_id];
		}
		inline const Vect<Vect<uintFragCount>> &MeanErrorRateForGCPerTileReference(uintTempSeq template_segment, uintTileId tile_id) const{
			return mean_error_rate_for_gc_per_tile_reference_.at(template_segment)[tile_id];
		}
		inline const Vect<Vect<uintFragCount>> &MeanErrorRateForFragmentLengthPerTileReference(uintTempSeq template_segment, uintTileId tile_id) const{
			return mean_error_rate_for_fragment_length_per_tile_reference_.at(template_segment)[tile_id];
		}
		inline const Vect<Vect<uintFragCount>> &GCForFragmentLengthPerTileReference(uintTempSeq template_segment, uintTileId tile_id) const{
			return gc_for_fragment_length_per_tile_reference_.at(template_segment)[tile_id];
		}

		inline const Vect<Vect<uintFragCount>> &SequenceQualityMeanPaired(uintTileId tile_id) const{
			return sequence_quality_mean_paired_per_tile_.at(tile_id);
		}
		inline const Vect<uintFragCount> &SequenceQualityProbabilityMean(uintTempSeq template_segment) const{
			return sequence_quality_probability_mean_.at(template_segment);
		}
		inline const Vect<uintFragCount> &SequenceQualityMinimum(uintTempSeq template_segment) const{
			return sequence_quality_minimum_.at(template_segment);
		}
		inline const Vect<uintFragCount> &SequenceQualityFirstQuartile(uintTempSeq template_segment) const{
			return sequence_quality_first_quartile_.at(template_segment);
		}
		inline const Vect<uintFragCount> &SequenceQualityMedian(uintTempSeq template_segment) const{
			return sequence_quality_median_.at(template_segment);
		}
		inline const Vect<uintFragCount> &SequenceQualityThirdQuartile(uintTempSeq template_segment) const{
			return sequence_quality_third_quartile_.at(template_segment);
		}
		inline const Vect<uintFragCount> &SequenceQualityMaximum(uintTempSeq template_segment) const{
			return sequence_quality_maximum_.at(template_segment);
		}
		inline const Vect<Vect<uintFragCount>> &SequenceQualityContent(uintTempSeq template_segment) const{
			return sequence_quality_content_.at(template_segment);
		}

		inline const Vect<Vect<uintNucCount>> &HomoqualityDistribution() const{
			return homoquality_distribution_;
		}
		inline const SeqQualityStats<uintNucCount> &NucleotideQuality(uintTempSeq template_segment, uintBaseCall nucleotide) const{
			return nucleotide_quality_.at(template_segment).at(nucleotide);
		}

		inline const Vect<SeqQualityStats<uintNucCount>> &BaseQualityStatsReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall ref_base) const{
			return base_quality_stats_per_tile_reference_.at(template_segment).at(ref_base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &ErrorRateForPositionReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return error_rate_for_position_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}
		inline const Vect<Vect<uintNucCount>> &BaseQualityForErrorRateReference(uintTempSeq template_segment, uintTileId tile_id, uintBaseCall base) const{
			return base_quality_for_error_rate_per_tile_reference_.at(template_segment).at(base)[tile_id];
		}

		inline const Vect<SeqQualityStats<uintNucCount>> &BaseQualityStatsReference(uintTempSeq template_segment) const{
			return base_quality_stats_reference_.at(template_segment);
		}
		inline const Vect<double> &BaseQualityMeanReference(uintTempSeq template_segment) const{
			return base_quality_mean_reference_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityMinimumReference(uintTempSeq template_segment) const{
			return base_quality_minimum_reference_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityFirstQuartileReference(uintTempSeq template_segment) const{
			return base_quality_first_quartile_reference_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityMedianReference(uintTempSeq template_segment) const{
			return base_quality_median_reference_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityThirdQuartileReference(uintTempSeq template_segment) const{
			return base_quality_third_quartile_reference_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityMaximumReference(uintTempSeq template_segment) const{
			return base_quality_maximum_reference_.at(template_segment);
		}

		inline const Vect<uintQual> &AverageSequenceQualityForGC(uintTempSeq template_segment) const{
			return average_sequence_quality_for_gc_.at(template_segment);
		}

		inline const Vect<SeqQualityStats<uintNucCount>> &BaseQualityStats(uintTempSeq template_segment) const{
			return base_quality_stats_.at(template_segment);
		}
		inline const Vect<double> &BaseQualityMean(uintTempSeq template_segment) const{
			return base_quality_mean_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityMinimum(uintTempSeq template_segment) const{
			return base_quality_minimum_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityFirstQuartile(uintTempSeq template_segment) const{
			return base_quality_first_quartile_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityMedian(uintTempSeq template_segment) const{
			return base_quality_median_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityThirdQuartile(uintTempSeq template_segment) const{
			return base_quality_third_quartile_.at(template_segment);
		}
		inline const Vect<uintQual> &BaseQualityMaximum(uintTempSeq template_segment) const{
			return base_quality_maximum_.at(template_segment);
		}
		inline const Vect<Vect<intQualDiff>> &TileQualityMeanDifference(uintTempSeq template_segment) const{
			return tile_quality_mean_difference_.at(template_segment);
		}
		inline const Vect<double> &BaseQualityMeanPerStrand(uintTempSeq strand) const{
			return base_quality_mean_per_strand_.at(strand);
		}

		inline const Vect<Vect<uintNucCount>>  &BaseQualityForSequence(uintTempSeq template_segment) const{
			return base_quality_for_sequence_.at(template_segment);
		}
		inline const Vect<Vect<uintNucCount>>  &BaseQualityForPrecedingQuality(uintTempSeq template_segment) const{
			return base_quality_for_preceding_quality_.at(template_segment);
		}

		inline const Vect<uintFragCount> &SequenceQualityMean(uintTempSeq template_segment) const{
			return sequence_quality_mean_.at(template_segment);
		}
		inline const Vect<Vect<uintFragCount>> &SequenceQualityMeanPaired() const{
			return sequence_quality_mean_paired_;
		}
		inline const Vect<uintQual> &MeanSequenceQualityMeanByFragmentLength(uintTempSeq template_segment) const{
			return mean_sequence_quality_mean_by_fragment_length_.at(template_segment);
		}
		inline const Vect<uintQual> &AverageSequenceQualityForBase(uintTempSeq template_segment, uintBaseCall nucleotide) const{
			return average_sequence_quality_for_base_.at(template_segment).at(nucleotide);
		}
		
		// Setter functions
		inline void AddRefBase(
				uintTempSeq template_segment,
				uintBaseCall ref_base,
				uintBaseCall dom_error,
				uintTileId tile_id,
				uintQual quality,
				uintPercent error_rate,
				uintQual last_qual,
				uintQual seq_qual,
				uintReadLen read_pos){
			// For estimation
			++tmp_base_quality_stats_per_tile_per_error_reference_.at(template_segment).at(ref_base).at(dom_error).at(tile_id).at(read_pos).at(quality);
			++tmp_error_rate_for_position_per_tile_per_error_reference_.at(template_segment).at(ref_base).at(dom_error).at(tile_id).at(read_pos).at(error_rate);
			++tmp_base_quality_for_error_rate_per_tile_per_error_reference_.at(template_segment).at(ref_base).at(dom_error).at(tile_id).at(error_rate).at(quality);
			++tmp_base_quality_for_preceding_quality_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(last_qual).at(quality);
			++tmp_preceding_quality_for_error_rate_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(error_rate).at(last_qual);
			++tmp_preceding_quality_for_position_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(read_pos).at(last_qual);
			++tmp_base_quality_for_sequence_quality_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(seq_qual).at(quality);
			++tmp_preceding_quality_for_sequence_quality_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(seq_qual).at(last_qual);
			++tmp_sequence_quality_for_error_rate_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(error_rate).at(seq_qual);
			++tmp_sequence_quality_for_position_per_tile_reference_.at(template_segment).at(ref_base).at(tile_id).at(read_pos).at(seq_qual);

			if(kWriteOut4dMatrixCsvs){
				if(0 == template_segment && 0 == ref_base && 0 == tile_id && 0 == error_rate){
					++tmp_csv_seg0_a_base_quality_preceding_quality_sequence_quality_position_.at(quality).at(last_qual).at(seq_qual).at(read_pos);
				}
				if(1 == template_segment && 1 == ref_base && 0 == tile_id && 37 == seq_qual){
					++tmp_csv_seg1_c_base_quality_preceding_quality_error_rate_position_.at(quality).at(last_qual).at(error_rate).at(read_pos);
				}
			}
		}
		inline void AddRefRead(uintTempSeq template_segment, uintTileId tile_id, uintPercent gc, uintQual seq_qual_mean, uintPercent mean_error_rate, uintSeqLen fragment_length){
			++tmp_sequence_quality_mean_for_gc_per_tile_reference_.at(template_segment).at(tile_id).at(gc).at(seq_qual_mean);
			++tmp_sequence_quality_mean_for_mean_error_rate_per_tile_reference_.at(template_segment).at(tile_id).at(mean_error_rate).at(seq_qual_mean);
			++tmp_sequence_quality_mean_for_fragment_length_per_tile_reference_.at(template_segment).at(tile_id).at(fragment_length/kSqFragmentLengthBinSize).at(seq_qual_mean);
			++tmp_mean_error_rate_for_gc_per_tile_reference_.at(template_segment).at(tile_id).at(gc).at(mean_error_rate);
			++tmp_mean_error_rate_for_fragment_length_per_tile_reference_.at(template_segment).at(tile_id).at(fragment_length/kSqFragmentLengthBinSize).at(mean_error_rate);
			++tmp_gc_for_fragment_length_per_tile_reference_.at(template_segment).at(tile_id).at(fragment_length/kSqFragmentLengthBinSize).at(gc);
		}

		inline void AddRawBase(
				uintTempSeq template_segment,
				uintBaseCall called_base,
				uintTileId tile_id,
				uintTempSeq strand,
				uintQual quality,
				uintQual seq_qual,
				uintQual last_qual,
				uintReadLen read_pos){
			// For plotting
			++tmp_base_quality_for_sequence_per_tile_.at(template_segment).at(called_base).at(tile_id).at(seq_qual).at(quality);
			++tmp_base_quality_stats_per_tile_.at(template_segment).at(called_base).at(tile_id).at(read_pos).at(quality);
			++tmp_sequence_quality_for_position_per_tile_.at(template_segment).at(called_base).at(tile_id).at(read_pos).at(seq_qual);
			++tmp_base_quality_for_preceding_quality_per_tile_.at(template_segment).at(called_base).at(tile_id).at(last_qual).at(quality);
			++tmp_preceding_quality_for_sequence_per_tile_.at(template_segment).at(called_base).at(tile_id).at(seq_qual).at(last_qual);
			++tmp_preceding_quality_for_position_per_tile_.at(template_segment).at(called_base).at(tile_id).at(read_pos).at(last_qual);

			if(strand < 2){
				++tmp_base_quality_stats_per_strand_.at(strand).at(read_pos).at(quality);
			}
			++tmp_nucleotide_quality_.at(template_segment).at(called_base).at(quality);
		}
		inline void AddRawHomoqualimer(uintQual quality, uintReadLen length){
			++tmp_homoquality_distribution_.at(quality).at(length);
		}
		inline void AddRawRead(
				uintQual &paired_seq_qual,
				const SeqQualityStats<uintNucCount> &seq_qual_stats,
				uintTempSeq template_segment,
				uintTileId tile_id,
				std::array<uintSeqLen, 5> &read_bases,
				uintSeqLen read_length){
			for( auto qual = seq_qual_stats.from(); qual < seq_qual_stats.to(); ++qual){
				if(seq_qual_stats.at(qual)){ // So that qualities that do not exist are not filled in
					++tmp_sequence_quality_content_.at(template_segment).at(qual).at(seq_qual_stats.at(qual));
				}
			}
			++tmp_sequence_quality_minimum_.at(template_segment).at(seq_qual_stats.minimum_);
			++tmp_sequence_quality_first_quartile_.at(template_segment).at(seq_qual_stats.first_quartile_);
			++tmp_sequence_quality_median_.at(template_segment).at(seq_qual_stats.median_);
			++tmp_sequence_quality_third_quartile_.at(template_segment).at(seq_qual_stats.third_quartile_);
			++tmp_sequence_quality_maximum_.at(template_segment).at(seq_qual_stats.maximum_);

			if( paired_seq_qual ){
				if(template_segment){
					++tmp_sequence_quality_mean_paired_per_tile_.at(tile_id).at(paired_seq_qual).at(seq_qual_stats.mean_);
				}
				else{
					++tmp_sequence_quality_mean_paired_per_tile_.at(tile_id).at(seq_qual_stats.mean_).at(paired_seq_qual);
				}
			}
			else{
				paired_seq_qual = seq_qual_stats.mean_;
			}

			for(auto nuc=tmp_sequence_quality_for_base_per_tile_.at(template_segment).size(); nuc--; ){
				++tmp_sequence_quality_for_base_per_tile_.at(template_segment).at(nuc).at(tile_id).at(reseq::utilities::Percent( read_bases.at(nuc), read_length )).at(seq_qual_stats.mean_);
			}
			++tmp_sequence_quality_mean_for_gc_per_tile_.at(template_segment).at(tile_id).at(reseq::utilities::Percent( read_bases.at(1)+read_bases.at(2)+read_bases.at(4)/2, read_length )).at(seq_qual_stats.mean_);

			++tmp_sequence_quality_probability_mean_.at(template_segment).at(seq_qual_stats.probability_mean_);
		}

		// Main functions
		void Prepare(uintTileId num_tiles, uintQual size_qual, uintReadLen size_pos, uintSeqLen maximum_fragment_length);
		void RemoveSystematicUpTo(uintPercent max_rate);
		void Finalize(uintFragCount total_number_reads);
		void Shrink();
		void PrepareEstimation();
		void PreparePlotting();
		void PrepareTesting();
	};
}

#endif // QUALITYSTATS_H
