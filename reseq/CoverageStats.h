#ifndef COVERAGESTATS_H
#define COVERAGESTATS_H

#include <algorithm>
#include <atomic>
#include <array>
#include <cmath>
#include <deque>
#include <mutex>
#include <stdint.h>
#include <utility>

#include "reportingUtils.hpp"

#include <seqan/bam_io.h>

#include "ErrorStats.h"
#include "QualityStats.h"
#include "Reference.h"
#include "utilities.hpp"
#include "Vect.hpp"

namespace reseq{
	class CoverageStats{
	public:
		struct FullRecord{
			seqan::BamAlignmentRecord record_;
			uintSeqLen from_ref_pos_;
			uintSeqLen to_ref_pos_;
			uintTileId tile_id_;
			uintQual sequence_quality_;
			uintPercent reference_gc_;
			uintSeqLen fragment_length_;

			FullRecord():
				from_ref_pos_(0),
				to_ref_pos_(0){
			}
		};

		struct ErrorInfo{
			seqan::Dna5 base_;
			uintPercent rate_;
			bool valid_and_coverage_sufficient_;

			ErrorInfo(seqan::Dna5 base, uintPercent rate, bool coverage_sufficient):
				base_(base),
				rate_(rate),
				valid_and_coverage_sufficient_(coverage_sufficient){
			}
		};

		struct CoveragePosition{
			std::array<std::atomic<uintCovCount>, 5> coverage_forward_;
			std::array<std::atomic<uintCovCount>, 5> coverage_reverse_;

			std::array<seqan::Dna5, 2> dom_error_; // dom_error_[strand] = domError
			std::array<uintPercent, 2> error_rate_; // error_rate[strand] = errorRate

			bool valid_;

			CoveragePosition():
				valid_(true){
				for( int i=5; i--; ){
					coverage_forward_.at(i) = 0;
					coverage_reverse_.at(i) = 0;
				}
			}

			CoveragePosition(const CoveragePosition &UNUSED(right)){
				throw std::runtime_error( "This function should never be called." );
			}
		};

		struct ProcessedCoveragePosition{
			std::array<seqan::Dna5, 2> dom_error_; // dom_error_[strand] = domError
			std::array<uintPercent, 2> error_rate_; // error_rate[strand] = errorRate

			bool valid_;

			ProcessedCoveragePosition(const CoveragePosition &rhs):
				dom_error_(rhs.dom_error_),
				error_rate_(rhs.error_rate_),
				valid_(rhs.valid_)
			{}
		};

		struct CoverageBlock{
			uintRefSeqId sequence_id_;
			uintSeqLen start_pos_;
			CoverageBlock *previous_block_;
			std::atomic<CoverageBlock *> next_block_;
			std::vector<CoveragePosition> coverage_;
			std::vector<ProcessedCoveragePosition> previous_coverage_; // End of coverage information from previous_block_ (max read length dependent)
			std::atomic<uintFragCount> unprocessed_fragments_;
			std::vector<FullRecord *> reads_;
			intVariantId first_variant_id_;

			CoverageBlock(uintRefSeqId seq_id, uintSeqLen start_pos, CoverageBlock *prev_block):
				sequence_id_(seq_id),
				start_pos_(start_pos),
				previous_block_(prev_block),
				next_block_(NULL),
				unprocessed_fragments_(0),
				first_variant_id_(0){
			}
		};

	private:
		// Definitions
		static const uintSeqLen kBlockSize = 10000;
		static const uintCovCount kMaxCoverage = 10000; // Maximum counted coverage (Only relevant for plotting)
		static constexpr double kRejectingBinomialP = 0.01; // If the probability of having the given number of errors is lower than this value the errors at this position are called systematic

		// Data dependent parameter
		uintCovCount coverage_threshold_; // Minimium coverage to accept value from this position into error_rates_ and dominant_errors_
		uintSeqLen reset_distance_; // Maximum distance between two systematic errors to continue error region
		uintSeqLen gc_range_;
		uintReadLen maximum_read_length_on_reference_;

		// Mutex
		std::mutex clean_up_mutex_;
		std::mutex reuse_mutex_;
		std::mutex variant_loading_mutex_;

		// Temporary variables
		std::array<std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,4>,5>,4> tmp_dominant_errors_by_distance_;
		std::array<std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,4>,5>,4> tmp_dominant_errors_by_gc_;
		std::array<std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,4>,5>,4> tmp_gc_by_distance_de_;
		std::array<std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,4>,5>,4> tmp_dominant_errors_by_start_rates_;
		std::array<std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,4>,5>,4> tmp_start_rates_by_distance_de_;
		std::array<std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,4>,5>,4> tmp_start_rates_by_gc_de_;

		std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,5>,4> tmp_error_rates_by_distance_;
		std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,5>,4> tmp_error_rates_by_gc_;
		std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,5>,4> tmp_gc_by_distance_er_;
		std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,5>,4> tmp_error_rates_by_start_rates_;
		std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,5>,4> tmp_start_rates_by_distance_er_;
		std::array<std::array<std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>>,5>,4> tmp_start_rates_by_gc_er_;

		std::vector<utilities::VectorAtomic<uintNucCount>> tmp_coverage_;
		std::array<std::vector<utilities::VectorAtomic<uintNucCount>>, 2> tmp_coverage_stranded_;
		std::array<std::vector<utilities::VectorAtomic<uintNucCount>>, 2> tmp_coverage_stranded_percent_;
		std::array<std::vector<utilities::VectorAtomic<uintNucCount>>, 2> tmp_coverage_stranded_percent_min_cov_10_;
		std::array<std::vector<utilities::VectorAtomic<uintNucCount>>, 2> tmp_coverage_stranded_percent_min_cov_20_;
		std::vector<utilities::VectorAtomic<uintNucCount>> tmp_error_coverage_;
		std::vector<utilities::VectorAtomic<uintNucCount>> tmp_error_coverage_percent_;
		std::vector<utilities::VectorAtomic<uintNucCount>> tmp_error_coverage_percent_min_cov_10_;
		std::vector<utilities::VectorAtomic<uintNucCount>> tmp_error_coverage_percent_min_cov_20_;
		std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>> tmp_error_coverage_percent_stranded_;
		std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>> tmp_error_coverage_percent_stranded_min_strand_cov_10_;
		std::vector<std::vector<utilities::VectorAtomic<uintNucCount>>> tmp_error_coverage_percent_stranded_min_strand_cov_20_;

		// Collected variables for simulation
		std::array<std::array<std::array<Vect<Vect<uintNucCount>>,4>,5>,4> dominant_errors_by_distance_; // dominant_errors_by_distance_[refBase][previousRefBase][domRefBaseLast5][(distanceToStartOfErrorRegion+9)/10][dominantError] = #refBases
		std::array<std::array<std::array<Vect<Vect<uintNucCount>>,4>,5>,4> dominant_errors_by_gc_; // dominant_errors_by_gc_[refBase][previousRefBase][domRefBaseLast5][GClastHalfAverageReadLength][dominantError] = #refBases
		std::array<std::array<std::array<Vect<Vect<uintNucCount>>,4>,5>,4> gc_by_distance_de_; // gc_by_distance_de_[refBase][previousRefBase][domRefBaseLast5][(distanceToStartOfErrorRegion+9)/10][GClastHalfAverageReadLength] = #refBases
		std::array<std::array<std::array<Vect<Vect<uintNucCount>>,4>,5>,4> dominant_errors_by_start_rates_; // dominant_errors_by_start_rates_[refBase][previousRefBase][domRefBaseLast5][errorRateStart][dominantError] = #refBases
		std::array<std::array<std::array<Vect<Vect<uintNucCount>>,4>,5>,4> start_rates_by_distance_de_; // start_rates_by_distance_de_[refBase][previousRefBase][domRefBaseLast5][(distanceToStartOfErrorRegion+9)/10][errorRateStart] = #refBases
		std::array<std::array<std::array<Vect<Vect<uintNucCount>>,4>,5>,4> start_rates_by_gc_de_; // start_rates_by_gc_de_[refBase][previousRefBase][domRefBaseLast5][(distanceToStartOfErrorRegion+9)/10][errorRateStart] = #refBases

		std::array<std::array<Vect<Vect<uintNucCount>>,5>,4> error_rates_by_distance_; // error_rates_by_distance_[refBase][dominantError][(distanceToStartOfErrorRegion+9)/10][errorRate] = #refBases
		std::array<std::array<Vect<Vect<uintNucCount>>,5>,4> error_rates_by_gc_; // error_rates_by_gc_[refBase][dominantError][GClastHalfAverageReadLength][errorRate] = #refBases
		std::array<std::array<Vect<Vect<uintNucCount>>,5>,4> gc_by_distance_er_; // gc_by_distance_er_[refBase][dominantError][(distanceToStartOfErrorRegion+9)/10][GClastHalfAverageReadLength] = #refBases
		std::array<std::array<Vect<Vect<uintNucCount>>,5>,4> error_rates_by_start_rates_; // error_rates_by_start_rates_[refBase][dominantError][errorRateStart][errorRate] = #refBases
		std::array<std::array<Vect<Vect<uintNucCount>>,5>,4> start_rates_by_distance_er_; // start_rates_by_distance_[refBase][dominantError][(distanceToStartOfErrorRegion+9)/10][errorRateStart] = #refBases
		std::array<std::array<Vect<Vect<uintNucCount>>,5>,4> start_rates_by_gc_er_; // start_rates_by_gc_[refBase][dominantError][GClastHalfAverageReadLength][errorRateStart] = #refBases

		// Collected variables for plotting
		Vect<uintSurBlockId> block_error_rate_;
		Vect<uintSurBlockId> block_non_systematic_error_rate_;

		Vect<uintNucCount> coverage_; // coverage_[ coverageDepth ] = #bases
		std::array<Vect<uintNucCount>, 2> coverage_stranded_; // coverage_stranded_[forward/reverse][ coverageDepthOnStrand ] = #bases
		std::array<Vect<uintNucCount>, 2> coverage_stranded_percent_; // coverage_stranded_percent_[forward/reverse][ coverageDepthOnStrand/coverageDepth*100 ] = #bases
		std::array<Vect<uintNucCount>, 2> coverage_stranded_percent_min_cov_10_; // coverage_stranded_percent_min_cov_10_[forward/reverse][ coverageDepthOnStrand/coverageDepth*100 ] = #bases
		std::array<Vect<uintNucCount>, 2> coverage_stranded_percent_min_cov_20_; // coverage_stranded_percent_min_cov_20_[forward/reverse][ coverageDepthOnStrand/coverageDepth*100 ] = #bases
		Vect<uintNucCount> error_coverage_; // error_coverage_[ #errorsAtReferencePosition ] = #bases
		Vect<uintNucCount> error_coverage_percent_; // error_coverage_percent_[ #errorsAtReferencePosition/coverage*100 ] = #bases
		Vect<uintNucCount> error_coverage_percent_min_cov_10_; // error_coverage_percent_min_cov_10_[ #errorsAtReferencePosition/coverage*100 ] = #bases
		Vect<uintNucCount> error_coverage_percent_min_cov_20_; // error_coverage_percent_min_cov_20_[ #errorsAtReferencePosition/coverage*100 ] = #bases
		Vect<Vect<uintNucCount>> error_coverage_percent_stranded_; // error_coverage_percent_stranded_[ #forwardErrorsAtReferencePosition/coverage*100 ][ #reverseErrorsAtReferencePosition/coverage*100 ] = #bases
		Vect<Vect<uintNucCount>> error_coverage_percent_stranded_min_strand_cov_10_; // error_coverage_percent_stranded_min_strand_cov_10_[ #forwardErrorsAtReferencePosition/coverage*100 ][ #reverseErrorsAtReferencePosition/coverage*100 ] = #bases
		Vect<Vect<uintNucCount>> error_coverage_percent_stranded_min_strand_cov_20_; // error_coverage_percent_stranded_min_strand_cov_20_[ #forwardErrorsAtReferencePosition/coverage*100 ][ #reverseErrorsAtReferencePosition/coverage*100 ] = #bases

		// Calculated variables for plotting
		Vect<Vect<uintNucCount>> error_rates_by_distance_sum_; // error_rates_by_distance_sum_[(distanceToStartOfErrorRegion+9)/10][errorRate] = #refBases
		Vect<Vect<uintNucCount>> error_rates_by_gc_sum_; // error_rates_by_gc_sum_[GClastHalfAverageReadLength][errorRate] = #refBases

		// Temporary variables for read in
		CoverageBlock *first_block_;
		std::atomic<CoverageBlock *> last_block_;
		std::vector<CoverageBlock *> reusable_blocks_;
		//std::array<std::pair<std::atomic_flag, CoverageBlock *>, kMaxKeptBlocks> reusable_blocks_;
		intFragCountShift zero_coverage_region_; // We need the possibility for negative numbers as from the total zero_coverage_region_ we subtract excluded zero coverage regions at the beginning and end of read, which are not necessarily added before due to fixed block length that reaches past the last read in a reference sequence

		std::vector<ErrorInfo> tmp_errors_forward_;
		std::vector<ErrorInfo> tmp_errors_reverse_;
		uintSeqLen distance_to_start_of_error_region_forward_;
		uintPercent start_rate_forward_;

		// Helper functions
		inline bool CoveragePosValid(intSeqShift coverage_pos, CoverageStats::CoverageBlock *coverage_block) const{
			if(0 > coverage_pos){
				return coverage_block->previous_coverage_.at(-1*coverage_pos).valid_;
			}
			else{
				return coverage_block->coverage_.at(coverage_pos).valid_;
			}
		}
		void EvalRead(FullRecord *record, CoverageStats::CoverageBlock *coverage_block, const Reference &reference, QualityStats &qualities, ErrorStats &errors, uintQual phred_quality_offset);

		inline void CountEmptyEndOfSequence(const Reference &reference){
			zero_coverage_region_ += reference.SequenceLength((*last_block_).sequence_id_) - (*last_block_).start_pos_ - (*last_block_).coverage_.size();
		}
		inline void UpdateZeroCoverageRegion(intFragCountShift zero_coverage_length);
		uintCovCount GetNormalErrorCount(double error_rate, uintCovCount coverage);
		std::pair<double, double> GetMaxNonSystematicError(CoverageBlock *block, const Reference &reference);
		inline reseq::uintCovCount GetMaxNonSystematicError(std::pair<double, double> non_systematic_error, uintCovCount coverage){
			return std::max(static_cast<uintCovCount>(1), static_cast<uintCovCount>(std::round( non_systematic_error.first + coverage*non_systematic_error.second )));
		}
		void UpdateCoverageAtSinglePosition(CoveragePosition &coverage, seqan::Dna5 ref_base, std::pair<double, double> non_systematic_error);
		
		CoverageBlock *CreateBlock(uintRefSeqId seq_id, uintSeqLen start_pos);
		void UpdateFirstVariant(CoverageBlock &block, const Reference &reference);
		bool IsVariantPosition( intVariantId &cur_var, uintRefSeqId ref_seq_id, uintSeqLen pos, const Reference &reference ) const;
		void InitBlock(CoverageBlock &block, const Reference &reference);
		void ProcessBlock(CoverageBlock *block, const Reference &reference);
		void CountBlock(CoverageBlock *block, const Reference &reference);
		CoverageBlock *RemoveBlock(CoverageBlock *block);

		// Boost archive functions
		friend class boost::serialization::access;
		template<class Archive> void serialize(Archive & ar, const unsigned int UNUSED(version)){
			ar & coverage_threshold_;
			ar & reset_distance_;

			ar & dominant_errors_by_distance_;
			ar & dominant_errors_by_gc_;
			ar & gc_by_distance_de_;
			ar & dominant_errors_by_start_rates_;
			ar & start_rates_by_distance_de_;
			ar & start_rates_by_gc_de_;

			ar & error_rates_by_distance_;
			ar & error_rates_by_gc_;
			ar & gc_by_distance_er_;
			ar & error_rates_by_start_rates_;
			ar & start_rates_by_distance_er_;
			ar & start_rates_by_gc_er_;

			ar & block_error_rate_;
			ar & block_non_systematic_error_rate_;

			ar & coverage_;
			ar & coverage_stranded_;
			ar & coverage_stranded_percent_;
			ar & coverage_stranded_percent_min_cov_10_;
			ar & coverage_stranded_percent_min_cov_20_;
			ar & error_coverage_;
			ar & error_coverage_percent_;
			ar & error_coverage_percent_min_cov_10_;
			ar & error_coverage_percent_min_cov_20_;
			ar & error_coverage_percent_stranded_;
			ar & error_coverage_percent_stranded_min_strand_cov_10_;
			ar & error_coverage_percent_stranded_min_strand_cov_20_;
		}

		// Google test
		friend class CoverageStatsTest;
		friend class ProbabilityEstimatesTest;

	public:
		CoverageStats():
			coverage_threshold_(10),
			first_block_(NULL),
			last_block_(NULL),
			zero_coverage_region_(0){
		}

		// Getter functions
		inline const Vect<Vect<uintNucCount>> &DominantErrorsByDistance(seqan::Dna ref_base, seqan::Dna5 last_ref_base, seqan::Dna5 dom_last5) const{
			return dominant_errors_by_distance_.at(ref_base).at(last_ref_base).at(dom_last5);
		}
		inline const Vect<Vect<uintNucCount>> &DominantErrorsByGC(seqan::Dna ref_base, seqan::Dna5 last_ref_base, seqan::Dna5 dom_last5) const{
			return dominant_errors_by_gc_.at(ref_base).at(last_ref_base).at(dom_last5);
		}
		inline const Vect<Vect<uintNucCount>> &GCByDistance(seqan::Dna ref_base, seqan::Dna5 last_ref_base, seqan::Dna5 dom_last5) const{
			return gc_by_distance_de_.at(ref_base).at(last_ref_base).at(dom_last5);
		}
		inline const Vect<Vect<uintNucCount>> &DominantErrorsByStartRates(seqan::Dna ref_base, seqan::Dna5 last_ref_base, seqan::Dna5 dom_last5) const{
			return dominant_errors_by_start_rates_.at(ref_base).at(last_ref_base).at(dom_last5);
		}
		inline const Vect<Vect<uintNucCount>> &StartRatesByDistance(seqan::Dna ref_base, seqan::Dna5 last_ref_base, seqan::Dna5 dom_last5) const{
			return start_rates_by_distance_de_.at(ref_base).at(last_ref_base).at(dom_last5);
		}
		inline const Vect<Vect<uintNucCount>> &StartRatesByGC(seqan::Dna ref_base, seqan::Dna5 last_ref_base, seqan::Dna5 dom_last5) const{
			return start_rates_by_gc_de_.at(ref_base).at(last_ref_base).at(dom_last5);
		}

		inline const Vect<Vect<uintNucCount>> &ErrorRatesByDistance(seqan::Dna ref_base, seqan::Dna5 dom_error) const{
			return error_rates_by_distance_.at(ref_base).at(dom_error);
		}
		inline const Vect<Vect<uintNucCount>> &ErrorRatesByGC(seqan::Dna ref_base, seqan::Dna5 dom_error) const{
			return error_rates_by_gc_.at(ref_base).at(dom_error);
		}
		inline const Vect<Vect<uintNucCount>> &GCByDistance(seqan::Dna ref_base, seqan::Dna5 dom_error) const{
			return gc_by_distance_er_.at(ref_base).at(dom_error);
		}
		inline const Vect<Vect<uintNucCount>> &ErrorRatesByStartRates(seqan::Dna ref_base, seqan::Dna5 dom_error) const{
			return error_rates_by_start_rates_.at(ref_base).at(dom_error);
		}
		inline const Vect<Vect<uintNucCount>> &StartRatesByDistance(seqan::Dna ref_base, seqan::Dna5 dom_error) const{
			return start_rates_by_distance_er_.at(ref_base).at(dom_error);
		}
		inline const Vect<Vect<uintNucCount>> &StartRatesByGC(seqan::Dna ref_base, seqan::Dna5 dom_error) const{
			return start_rates_by_gc_er_.at(ref_base).at(dom_error);
		}

		inline const Vect<Vect<uintNucCount>> &ErrorRatesByDistanceSum() const{
			return error_rates_by_distance_sum_;
		}
		inline const Vect<Vect<uintNucCount>> &ErrorRatesByGCSum() const{
			return error_rates_by_gc_sum_;
		}

		inline const Vect<uintSurBlockId> &BlockErrorRate() const{
			return block_error_rate_;
		}
		inline const Vect<uintSurBlockId> &BlockNonSystematicErrorRate() const{
			return block_non_systematic_error_rate_;
		}

		inline const Vect<uintNucCount> &Coverage() const{
			return coverage_;
		}
		inline const Vect<uintNucCount> &CoverageStranded( bool reverse_strand ) const{
			return coverage_stranded_.at(reverse_strand);
		}
		inline const Vect<uintNucCount> &CoverageStrandedPercent( bool reverse_strand ) const{
			return coverage_stranded_percent_.at(reverse_strand);
		}
		inline const Vect<uintNucCount> &CoverageStrandedPercentMinCov10( bool reverse_strand ) const{
			return coverage_stranded_percent_min_cov_10_.at(reverse_strand);
		}
		inline const Vect<uintNucCount> &CoverageStrandedPercentMinCov20( bool reverse_strand ) const{
			return coverage_stranded_percent_min_cov_20_.at(reverse_strand);
		}
		inline const Vect<uintNucCount> &ErrorCoverage() const{
			return error_coverage_;
		}
		inline const Vect<uintNucCount> &ErrorCoveragePercent() const{
			return error_coverage_percent_;
		}
		inline const Vect<uintNucCount> &ErrorCoveragePercentMinCov10() const{
			return error_coverage_percent_min_cov_10_;
		}
		inline const Vect<uintNucCount> &ErrorCoveragePercentMinCov20() const{
			return error_coverage_percent_min_cov_20_;
		}
		inline const Vect<Vect<uintNucCount>> &ErrorCoveragePercentStranded() const{
			return error_coverage_percent_stranded_;
		}
		inline const Vect<Vect<uintNucCount>> &ErrorCoveragePercentStrandedMinCov10() const{
			return error_coverage_percent_stranded_min_strand_cov_10_;
		}
		inline const Vect<Vect<uintNucCount>> &ErrorCoveragePercentStrandedMinCov20() const{
			return error_coverage_percent_stranded_min_strand_cov_20_;
		}
		
		// Setter functions
		inline void AddForward(uintSeqLen pos, CoverageBlock *block, seqan::Dna5 called_base){
			++(block->coverage_.at(pos).coverage_forward_.at(called_base));
		}
		inline void AddReverse(uintSeqLen pos, CoverageBlock *block, seqan::Dna5 called_base){
			++(block->coverage_.at(pos).coverage_reverse_.at(called_base));
		}
	
		// Main functions
		void Prepare(uintCovCount average_coverage, uintReadLen average_read_length, uintReadLen maximum_read_length_on_reference);

		CoverageBlock *FindBlock(uintRefSeqId ref_seq_id, uintSeqLen ref_pos);
		bool EnsureSpace(uintRefSeqId ref_seq_id, uintSeqLen start_pos, uintSeqLen end_pos, FullRecord *record, Reference &reference, uintReadLen minimum_sequence_length);

		void AddFragment( uintRefSeqId ref_seq_id, uintSeqLen ref_pos, CoverageBlock *&block );
		void RemoveFragment( uintRefSeqId ref_seq_id, uintSeqLen ref_pos, CoverageBlock *&block, uintFragCount &processed_fragments );

		static inline uintSeqLen GetStartPos(uintSeqLen pos, CoverageBlock *&start_block){
			while(pos >= start_block->start_pos_ + kBlockSize){
				start_block = start_block->next_block_;
			}
			return pos - start_block->start_pos_;
		}
		static inline void IncrementPos(uintSeqLen &pos, CoverageBlock *&block){
			if(++pos >= kBlockSize){
				pos = 0;
				block = block->next_block_;
			}
		}
		static inline void AddPos(uintSeqLen &pos, CoverageBlock *&block, uintSeqLen value){
			pos += value;
			while(pos >= kBlockSize){
				pos -= kBlockSize;
				block = block->next_block_;
			}
		}
		static inline void DecrementPos(uintSeqLen &pos, CoverageBlock *&block){
			if(0 == pos){
				pos = kBlockSize;
				block = block->previous_block_;
			}
			--pos;
		}
		static inline void SubtractPos(uintSeqLen &pos, CoverageBlock *&block, uintSeqLen value){
			if(value <= pos){
				pos -= value;
			}
			else{
				value -= pos;
				block = block->previous_block_;
				while(value > kBlockSize){
					value -= kBlockSize;
					block = block->previous_block_;
				}
				pos = kBlockSize-value;
			}
		}
		void UpdateDistances(uintSeqLen &distance_to_start_of_error_region, uintPercent &start_rate, uintPercent error_rate) const;
		uintRefSeqId CleanUp(uintSeqLen &still_needed_position, Reference &reference, QualityStats &qualities, ErrorStats &errors, uintQual phred_quality_offset, CoverageBlock *cov_block, uintFragCount &processed_fragments);
		bool PreLoadVariants(Reference &reference);
		bool Finalize(const Reference &reference, QualityStats &qualities, ErrorStats &errors, uintQual phred_quality_offset, uintReadLen minimum_sequence_length, std::mutex &print_mutex);
		inline void SetAllZero(const Reference &reference){
			zero_coverage_region_ = reference.TotalSize();
		}

		void Shrink();
		void PreparePlotting();
	};
}

#endif // COVERAGESTATS_H
