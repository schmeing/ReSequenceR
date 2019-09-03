#include "ReferenceTest.h"
using reseq::ReferenceTest;

#include <array>
using std::array;
#include <set>
using std::set;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include <seqan/seq_io.h>
using seqan::DnaString;

//include "utilities.h"
using reseq::utilities::VectorAtomic;

#include "CMakeConfig.h"

void ReferenceTest::TestVariantClass(){
	Reference::Variant test(0, "", 7);
	EXPECT_TRUE(test.InAllele(2));
	EXPECT_TRUE(test.InAllele(1));
	EXPECT_TRUE(test.InAllele(0));
	EXPECT_EQ(0, test.FirstAllele());

	test.allele_ = 6;
	EXPECT_TRUE(test.InAllele(2));
	EXPECT_TRUE(test.InAllele(1));
	EXPECT_FALSE(test.InAllele(0));
	EXPECT_EQ(1, test.FirstAllele());

	test.allele_ = 5;
	EXPECT_TRUE(test.InAllele(2));
	EXPECT_FALSE(test.InAllele(1));
	EXPECT_TRUE(test.InAllele(0));
	EXPECT_EQ(0, test.FirstAllele());

	test.allele_ = 4;
	EXPECT_TRUE(test.InAllele(2));
	EXPECT_FALSE(test.InAllele(1));
	EXPECT_FALSE(test.InAllele(0));
	EXPECT_EQ(2, test.FirstAllele());

	test.allele_ = 3;
	EXPECT_FALSE(test.InAllele(2));
	EXPECT_TRUE(test.InAllele(1));
	EXPECT_TRUE(test.InAllele(0));
	EXPECT_EQ(0, test.FirstAllele());

	test.allele_ = 2;
	EXPECT_FALSE(test.InAllele(2));
	EXPECT_TRUE(test.InAllele(1));
	EXPECT_FALSE(test.InAllele(0));
	EXPECT_EQ(1, test.FirstAllele());

	test.allele_ = 1;
	EXPECT_FALSE(test.InAllele(2));
	EXPECT_FALSE(test.InAllele(1));
	EXPECT_TRUE(test.InAllele(0));
	EXPECT_EQ(0, test.FirstAllele());
}

void ReferenceTest::TestInsertVariant(){
	ref_.variants_.resize(1);
	ref_.InsertVariant(0, 0, DnaString("A"), 1);
	ref_.InsertVariant(0, 1, DnaString("A"), 2);
	ref_.InsertVariant(0, 1, DnaString("ACT"), 1);
	ref_.InsertVariant(0, 1, DnaString("C"), 4);
	ref_.InsertVariant(0, 1, DnaString(""), 8);
	ref_.InsertVariant(0, 1, DnaString("C"), 16);
	ref_.InsertVariant(0, 1, DnaString(""), 32);
	ref_.InsertVariant(0, 1, DnaString("ACT"), 64);
	ref_.InsertVariant(0, 1, DnaString("TG"), 128);

	const vector<Reference::Variant> &vars(ref_.variants_.at(0));
	EXPECT_EQ(6, vars.size());
	for(uint32_t nvar=0; nvar < vars.size(); ++nvar){
		switch(nvar){
		case 0:
			EXPECT_EQ(0, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "A" );
			EXPECT_EQ(1, vars.at(nvar).allele_);
			break;
		case 1:
			EXPECT_EQ(1, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "" );
			EXPECT_EQ(40, vars.at(nvar).allele_);
			break;
		case 2:
			EXPECT_EQ(1, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "A" );
			EXPECT_EQ(2, vars.at(nvar).allele_);
			break;
		case 3:
			EXPECT_EQ(1, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "C" );
			EXPECT_EQ(20, vars.at(nvar).allele_);
			break;
		case 4:
			EXPECT_EQ(1, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "TG" );
			EXPECT_EQ(128, vars.at(nvar).allele_);
			break;
		case 5:
			EXPECT_EQ(1, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "ACT" );
			EXPECT_EQ(65, vars.at(nvar).allele_);
			break;
		}
	}

	ref_.variants_.clear();
}

void ReferenceTest::TestVariationLoading(){
	// freebayes -f GCF_000005845.2_ASM584v2_genomic.fna SRR490124.bam | awk 'BEGIN{pos[1]=1;pos[17]=1;pos[21]=1;pos[11369]=1;pos[953166]=1;pos[3192438]=1;pos[3424236]=1;pos[3424237]=1}("#" == substr($0,1,1) || $2 in pos)' > test-var.vcf
	// Manually changed alternative for pos 3192438 from GT to G to create deletion

	ASSERT_TRUE( ref_.PrepareVariantFile(std::string(PROJECT_SOURCE_DIR)+"/test/test-var.vcf") );
	ASSERT_TRUE( ref_.ReadFirstVariants() );

	EXPECT_EQ(2, ref_.num_alleles_);
	ASSERT_EQ(1, ref_.variants_.size());
	EXPECT_EQ(13, ref_.variants_.at(0).size());

	const vector<Reference::Variant> &vars(ref_.variants_.at(0));
	for(uint32_t nvar=0; nvar < vars.size(); ++nvar){
		switch(nvar){
		case 0:
			EXPECT_EQ(2, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "T" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 1:
			EXPECT_EQ(3, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "A" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 2:
			EXPECT_EQ(4, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "A" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 3:
			EXPECT_EQ(5, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "G" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 4:
			EXPECT_EQ(7, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "A" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 5:
			EXPECT_EQ(8, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "TTTTTCAGCTTTTCA" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 6:
			EXPECT_EQ(11368, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "T" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 7:
			EXPECT_EQ(11370, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "T" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 8:
			EXPECT_EQ(953165, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "G" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 9:
			EXPECT_EQ(3192437, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "G" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 10:
			EXPECT_EQ(3192438, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "" );
			EXPECT_FALSE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 11:
			EXPECT_EQ(3424235, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "C" );
			EXPECT_TRUE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		case 12:
			EXPECT_EQ(3424236, vars.at(nvar).position_);
			EXPECT_TRUE( vars.at(nvar).var_seq_ == "A" );
			EXPECT_TRUE( vars.at(nvar).InAllele(0) );
			EXPECT_TRUE( vars.at(nvar).InAllele(1) );
			break;
		}
	}

	ref_.ClearVariants(1);
	EXPECT_EQ(0, ref_.variants_.at(0).size());

	ref_.ClearAllVariants();
	EXPECT_EQ(0, ref_.variants_.size());
}

void ReferenceTest::TestVariationPositionLoading(){
	// freebayes -f GCF_000005845.2_ASM584v2_genomic.fna SRR490124.bam | awk 'BEGIN{pos[1]=1;pos[17]=1;pos[21]=1;pos[11369]=1;pos[953166]=1;pos[3192438]=1;pos[3424236]=1;pos[3424237]=1}("#" == substr($0,1,1) || $2 in pos)' > test-var.vcf
	// Manually changed alternative for pos 3192438 from GT to G to create deletion

	ASSERT_TRUE( ref_.PrepareVariantFile(std::string(PROJECT_SOURCE_DIR)+"/test/test-var.vcf") );
	ASSERT_TRUE( ref_.ReadFirstVariantPositions() );

	ASSERT_EQ(1, ref_.variant_positions_.size());

	TestVectorEquality({0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 20, 11368, 11369, 11370, 953165, 3192437, 3192438, 3424235, 3424236}, ref_.variant_positions_.at(0), "ref_.variant_positions_.at(0)", "is wrong", " for VariationPositionLoading.");

	ref_.ClearVariantPositions(1);
	EXPECT_EQ(0, ref_.variant_positions_.at(0).size());

	ref_.ClearAllVariantPositions();
	EXPECT_EQ(0, ref_.variant_positions_.size());
}

namespace reseq{
	TEST_F(ReferenceTest, Variants){
		ASSERT_TRUE( ref_.ReadFasta( (string(PROJECT_SOURCE_DIR)+"/test/ecoli-GCF_000005845.2_ASM584v2_genomic.fa").c_str() ) );

		TestVariantClass();
		TestInsertVariant();
		TestVariationLoading();
		TestVariationPositionLoading();
	}

	TEST_F(ReferenceTest, Functionality){
		ASSERT_TRUE( ref_.ReadFasta( (string(PROJECT_SOURCE_DIR)+"/test/reference-special-chars.fa").c_str() ) );
		for( auto base : ref_[0] ){
			EXPECT_TRUE(base == 'N') << "Special characters not handled properly\n";
		}

		ASSERT_TRUE( ref_.ReadFasta( (string(PROJECT_SOURCE_DIR)+"/test/reference-test.fa").c_str() ) );

		ASSERT_EQ(2, ref_.NumberSequences()) << "The loaded test reference does not have the right number of sequences\n";
		EXPECT_TRUE("NC_000913.3_1-500 bla" == ref_.ReferenceId(0)) << ref_.ReferenceId(0) << "\nThe first loaded test reference sequence does not have the correct name.\n";
		EXPECT_TRUE("NC_000913.3_10000-10500 blub" == ref_.ReferenceId(1)) << ref_.ReferenceId(1) << "\nThe second loaded test reference sequence does not have the correct name\n";
		EXPECT_TRUE("AGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGCAGCTTCTGAACTGGTTACCTGCCGTGAGTAAATTAAAATTTTATTGACTTAGGTCACTAAATACTTTAACCAATATAGGCATAGCGCACAGACAGATAAAAATTACAGAGTACACAACATCCATGAAACGCATTAGCACCACCATTACCACCACCATCACCATTACCACAGGTAACGGTGCGGGCTGACGCGTACAGGAAACACAGAAAAAAGCCCGCACCTGACAGTGCGGGCTTTTTTTTTCGACCAAAGGTAACGAGGTAACAACCATGCGAGTGTTGAAGTTCGGCGGTACATCAGTGGCAAATGCAGAACGTTTTCTGCGTGTTGCCGATATTCTGGAAAGCAATGCCAGGCAGGGGCAGGTGGCCACCGTCCTCTCTGCCCCCGCCAAAATCACCAACCACCTGGTGGCGATGATTGAAAAAACCAT" == ref_.ReferenceSequence(0)) << "The first loaded test reference sequence is not correct\n";
		EXPECT_TRUE("GATTGCGCTGGCACCGCAGATCAGCCCAATCCAGCCGGCAAAGTGGATGATTGCGGCGTTACCGGCAATGTTACCGATCGCCAGCAGGGCAAACAGCACGGTCAGGCTAAAGAAAACGAATTGCAGAACGCGTGCGCCTTTCAGCGTGCCGAAGAACATAAACAGCGTAAATACGCCCCACAGACCCAGGTAGACACCAAGGAACTGTGCATTTGGCGCATCGGTCAGACCCAGTTTCGGCATCAGCAGAATCGCAACCAGCGTCAGCCAGAAAGAACCGTAAGAGGTGAATGCGGTTAAACCGAAAGTGTTGCCTTTTTTGTACTCCAGCAGACCAGCAAAAATTTGCGCGATGCCGCCGTAGAAAATGCCCATGGCAAGAATAATACCGTCCAGAGCGAAATAACCCACGTTGTGCAGGTTAAGCAGAATGGTGGTCATGCCGAAGCCCATCAGGCCCAGCGGTGCCGGATTAGCCAACTTAGTGTTGCCCATAATTCC" == ref_[1]) << "The second loaded test reference sequence is not correct\n";
		EXPECT_EQ(500, ref_.SequenceLength(0)) << "SequenceLength function does not work\n";
		EXPECT_EQ(501, ref_.SequenceLength(1)) << "SequenceLength function does not work\n";

		DnaString test_string;
		ref_.ReferenceSequence(test_string, 0, 0, 10);
		EXPECT_TRUE("AGCTTTTCAT" == test_string) << test_string << "\nReferenceSequence function does return correct result\n";
		EXPECT_EQ(10, length(test_string)) << "ReferenceSequence function does return correct size\n";
		ref_.ReferenceSequence(test_string, 0, 500, 10, true);
		EXPECT_TRUE("ATGGTTTTTT" == test_string) << test_string << "\nReferenceSequence function does return correct reversed result\n";
		EXPECT_EQ(10, length(test_string)) << "ReferenceSequence function does return correct size if reversed\n";
		ref_.ReferenceSequence(test_string, 0, 0, 10, false, 20);
		EXPECT_TRUE(infix(test_string, 0, 10) == "AGCTTTTCAT") << test_string << "\nReferenceSequence function does return correct result with size specified\n";
		EXPECT_EQ(20, length(test_string)) << "ReferenceSequence function does return correct size with size specified\n";
		ref_.ReferenceSequence(test_string, 0, 500, 10, true, 20);
		EXPECT_TRUE(infix(test_string, 0, 10) == "ATGGTTTTTT") << test_string << "\nReferenceSequence function does return correct reversed result with size specified\n";
		EXPECT_EQ(20, length(test_string)) << "ReferenceSequence function does return correct size if reversed with size specified\n";

		vector<Reference::Variant> test_variants;
		test_variants.push_back({2, "", 2}); // Allele 1
		test_variants.push_back({4, "TAG", 3}); // Allele 0 + 1
		test_variants.push_back({9, "C", 1}); // Allele 0
		ref_.ReferenceSequence(test_string, 0, 0, 12, false, test_variants, {0, 0}, 0);
		EXPECT_TRUE("AGCTTAGTTCAC" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 0, 11, false, test_variants, {0, 0}, 1);
		EXPECT_TRUE("AGTTAGTTCAT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 4, 8, false, test_variants, {1, 0}, 1);
		EXPECT_TRUE("TAGTTCAT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 4, 7, false, test_variants, {1, 1}, 1);
		EXPECT_TRUE("AGTTCAT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 4, 6, false, test_variants, {1, 2}, 1);
		EXPECT_TRUE("GTTCAT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 10, 12, true, test_variants, {2, 0}, 0);
		EXPECT_TRUE("GTGAACTAAGCT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 10, 11, true, test_variants, {2, 0}, 1);
		EXPECT_TRUE("ATGAACTAACT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 5, 7, true, test_variants, {1, 0}, 0);
		EXPECT_TRUE("CTAAGCT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 5, 6, true, test_variants, {1, 2}, 0);
		EXPECT_TRUE("TAAGCT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 5, 5, true, test_variants, {1, 1}, 0);
		EXPECT_TRUE("AAGCT" == test_string) << test_string << "\n";

		test_variants.push_back({499, "TAG", 3}); // Allele 0 + 1
		ref_.ReferenceSequence(test_string, 0, 500, 12, true, test_variants, {3, 0}, 0);
		EXPECT_TRUE("CTATGGTTTTTT" == test_string) << test_string << "\n";
		ref_.ReferenceSequence(test_string, 0, 0, 11, false, test_variants, {0, 0}, 1, 20);
		EXPECT_TRUE(infix(test_string, 0, 11) == "AGTTAGTTCAT") << test_string << "\n";
		EXPECT_EQ(20, length(test_string));
		ref_.ReferenceSequence(test_string, 0, 500, 12, true, test_variants, {3, 0}, 0, 20);
		EXPECT_TRUE(infix(test_string, 0, 12) == "CTATGGTTTTTT") << test_string << "\n";
		EXPECT_EQ(20, length(test_string));

		string error_msg = "\nThe extraction of only the first part by cutting at the first space does not work\n";
		EXPECT_TRUE(ref_.ReferenceIdFirstPart(0) == "NC_000913.3_1-500") << ref_.ReferenceIdFirstPart(0) << error_msg;
		EXPECT_TRUE(ref_.ReferenceIdFirstPart(1) == "NC_000913.3_10000-10500") << ref_.ReferenceIdFirstPart(1) << error_msg;

		error_msg = "The function GCContentAbsolut returns wrong results\n";
		uint32_t n_count(0);
		EXPECT_EQ(2, ref_.GCContentAbsolut(n_count,0,0,7)) << error_msg;
		EXPECT_EQ(4, ref_.GCContentAbsolut(n_count,1,22,27)) << error_msg;

		error_msg = "The function GCContent returns wrong results\n";
		EXPECT_EQ(29, ref_.GCContent(0,0,7)) << error_msg;
		EXPECT_EQ(80, ref_.GCContent(1,22,27)) << error_msg;

		error_msg = "The function UpdateGC returns wrong results\n";
		uint64_t gc = ref_.GCContentAbsolut(n_count,0,0,7);
		ref_.UpdateGC(gc, n_count, ref_.ReferenceSequence(0), 0, 7);
		EXPECT_EQ(3, gc) << error_msg;
		ref_.UpdateGC(gc, n_count, ref_.ReferenceSequence(0), 1, 8);
		EXPECT_EQ(2, gc) << error_msg;
		ref_.UpdateGC(gc, n_count, ref_.ReferenceSequence(0), 2, 9);
		EXPECT_EQ(1, gc) << error_msg;
		ref_.UpdateGC(gc, n_count, ref_.ReferenceSequence(0), 3, 10);
		EXPECT_EQ(1, gc) << error_msg;
		gc = ref_.GCContentAbsolut(n_count, 1,4,10);
		ref_.UpdateGC(gc, n_count, ref_.ReferenceSequence(1), 4, 10);
		EXPECT_EQ(5, gc) << error_msg;
		gc = ref_.GCContentAbsolut(n_count, 1,22,27);
		ref_.UpdateGC(gc, n_count, ref_.ReferenceSequence(1), 22, 27);
		EXPECT_EQ(4, gc) << error_msg;
		EXPECT_EQ(0, n_count) << "In one of the GC tests above somehow a non-existing N was detected.";

		// cat reference-test.fa | seqtk seq | awk '(2==NR){print substr($0,length($0)-9,10) $0 substr($0,1,20)}' | awk 'BEGIN{split("0,1,2,15,497,498,499",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print i, pos, substr($0,i*10+pos+1,10)}}}' | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}{mult=1;sur=0;for(i=length($3);i>0;i-=1){sur+=mult*d[substr($3,i,1)];mult*=4}; print $1, $2, $3, sur}'
		error_msg = "The function ForwardSurrounding returns wrong results\n";
		array<uint32_t, 3> surrounding;
		ref_.ForwardSurrounding(surrounding,0,0);
		EXPECT_EQ(83, surrounding[0]) << error_msg;
		EXPECT_EQ(163795, surrounding[1]) << error_msg;
		EXPECT_EQ(909796, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,1);
		EXPECT_EQ(332, surrounding[0]) << error_msg;
		EXPECT_EQ(655183, surrounding[1]) << error_msg;
		EXPECT_EQ(493456, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,2);
		EXPECT_EQ(1330, surrounding[0]) << error_msg;
		EXPECT_EQ(523581, surrounding[1]) << error_msg;
		EXPECT_EQ(925249, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,15);
		EXPECT_EQ(1003384, surrounding[0]) << error_msg;
		EXPECT_EQ(495722, surrounding[1]) << error_msg;
		EXPECT_EQ(275383, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,497);
		EXPECT_EQ(1015809, surrounding[0]) << error_msg;
		EXPECT_EQ(313855, surrounding[1]) << error_msg;
		EXPECT_EQ(325511, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,498);
		EXPECT_EQ(917509, surrounding[0]) << error_msg;
		EXPECT_EQ(206845, surrounding[1]) << error_msg;
		EXPECT_EQ(253470, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,499);
		EXPECT_EQ(524308, surrounding[0]) << error_msg;
		EXPECT_EQ(827380, surrounding[1]) << error_msg;
		EXPECT_EQ(1013881, surrounding[2]) << error_msg;
		// cat reference-test.fa | seqtk seq | awk '(4==NR){print substr($0,length($0)-9,10) $0 substr($0,1,20)}' | awk 'BEGIN{split("17",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print i, pos, substr($0,i*10+pos+1,10)}}}' | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}{mult=1;sur=0;for(i=length($3);i>0;i-=1){sur+=mult*d[substr($3,i,1)];mult*=4}; print $1, $2, $3, sur}'
		ref_.ForwardSurrounding(surrounding,1,17);
		EXPECT_EQ(500825, surrounding[0]) << error_msg;
		EXPECT_EQ(144533, surrounding[1]) << error_msg;
		EXPECT_EQ(54422, surrounding[2]) << error_msg;

		error_msg = "The function UpdateForwardSurrounding returns wrong results\n";
		ref_.ForwardSurrounding(surrounding,0,0);
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(0), 1);
		EXPECT_EQ(332, surrounding[0]) << error_msg;
		EXPECT_EQ(655183, surrounding[1]) << error_msg;
		EXPECT_EQ(493456, surrounding[2]) << error_msg;
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(0), 2);
		EXPECT_EQ(1330, surrounding[0]) << error_msg;
		EXPECT_EQ(523581, surrounding[1]) << error_msg;
		EXPECT_EQ(925249, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,14);
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(0), 15);
		EXPECT_EQ(1003384, surrounding[0]) << error_msg;
		EXPECT_EQ(495722, surrounding[1]) << error_msg;
		EXPECT_EQ(275383, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,0,496);
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(0), 497);
		EXPECT_EQ(1015809, surrounding[0]) << error_msg;
		EXPECT_EQ(313855, surrounding[1]) << error_msg;
		EXPECT_EQ(325511, surrounding[2]) << error_msg;
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(0), 498);
		EXPECT_EQ(917509, surrounding[0]) << error_msg;
		EXPECT_EQ(206845, surrounding[1]) << error_msg;
		EXPECT_EQ(253470, surrounding[2]) << error_msg;
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(0), 499);
		EXPECT_EQ(524308, surrounding[0]) << error_msg;
		EXPECT_EQ(827380, surrounding[1]) << error_msg;
		EXPECT_EQ(1013881, surrounding[2]) << error_msg;
		ref_.ForwardSurrounding(surrounding,1,16);
		ref_.UpdateForwardSurrounding(surrounding, ref_.ReferenceSequence(1), 17);
		EXPECT_EQ(500825, surrounding[0]) << error_msg;
		EXPECT_EQ(144533, surrounding[1]) << error_msg;
		EXPECT_EQ(54422, surrounding[2]) << error_msg;

		// cat reference-test.fa | seqtk seq | awk '(2==NR){print substr($0,length($0)-19,20) $0 substr($0,1,10)}' | awk 'BEGIN{split("0,1,2,15,497,498,499",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print ">" i, pos; print substr($0,pos+22-i*10,10)}}}' | seqtk seq -r | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}(NR%2==1){pos=substr($0,2,length($0)-1)}(NR%2==0){mult=1;sur=0;for(i=length($0);i>0;i-=1){sur+=mult*d[substr($0,i,1)];mult*=4}; print pos, $0, sur}'
		error_msg = "The function ReverseSurrounding returns wrong results\n";
		ref_.ReverseSurrounding(surrounding,0,0);
		EXPECT_EQ(57353, surrounding[0]) << error_msg;
		EXPECT_EQ(846847, surrounding[1]) << error_msg;
		EXPECT_EQ(855350, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,1);
		EXPECT_EQ(538626, surrounding[0]) << error_msg;
		EXPECT_EQ(473855, surrounding[1]) << error_msg;
		EXPECT_EQ(1000269, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,2);
		EXPECT_EQ(134656, surrounding[0]) << error_msg;
		EXPECT_EQ(642751, surrounding[1]) << error_msg;
		EXPECT_EQ(1036499, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,15);
		EXPECT_EQ(613348, surrounding[0]) << error_msg;
		EXPECT_EQ(739384, surrounding[1]) << error_msg;
		EXPECT_EQ(10042, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,497);
		EXPECT_EQ(524915, surrounding[0]) << error_msg;
		EXPECT_EQ(720884, surrounding[1]) << error_msg;
		EXPECT_EQ(216468, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,498);
		EXPECT_EQ(917660, surrounding[0]) << error_msg;
		EXPECT_EQ(966653, surrounding[1]) << error_msg;
		EXPECT_EQ(54117, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,499);
		EXPECT_EQ(229415, surrounding[0]) << error_msg;
		EXPECT_EQ(241663, surrounding[1]) << error_msg;
		EXPECT_EQ(275673, surrounding[2]) << error_msg;
		// cat reference-test.fa | seqtk seq | awk '(4==NR){print substr($0,length($0)-19,20) $0 substr($0,1,10)}' | awk 'BEGIN{split("17",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print ">" i, pos; print substr($0,pos+22-i*10,10)}}}' | seqtk seq -r | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}(NR%2==1){pos=substr($0,2,length($0)-1)}(NR%2==0){mult=1;sur=0;for(i=length($0);i>0;i-=1){sur+=mult*d[substr($0,i,1)];mult*=4}; print pos, $0, sur}'
		ref_.ReverseSurrounding(surrounding,1,17);
		EXPECT_EQ(960397, surrounding[0]) << error_msg;
		EXPECT_EQ(945044, surrounding[1]) << error_msg;
		EXPECT_EQ(626906, surrounding[2]) << error_msg;

		error_msg = "The function UpdateReverseSurrounding returns wrong results\n";
		auto error_msg2 = "The function RollBackReverseSurrounding returns wrong results\n";
		ref_.ReverseSurrounding(surrounding,0,0);
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 1);
		EXPECT_EQ(538626, surrounding[0]) << error_msg;
		EXPECT_EQ(473855, surrounding[1]) << error_msg;
		EXPECT_EQ(1000269, surrounding[2]) << error_msg;
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 2);
		EXPECT_EQ(134656, surrounding[0]) << error_msg;
		EXPECT_EQ(642751, surrounding[1]) << error_msg;
		EXPECT_EQ(1036499, surrounding[2]) << error_msg;
		ref_.RollBackReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 1);
		EXPECT_EQ(538626, surrounding[0]) << error_msg2;
		EXPECT_EQ(473855, surrounding[1]) << error_msg2;
		EXPECT_EQ(1000269, surrounding[2]) << error_msg2;
		ref_.RollBackReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 0);
		EXPECT_EQ(57353, surrounding[0]) << error_msg2;
		EXPECT_EQ(846847, surrounding[1]) << error_msg2;
		EXPECT_EQ(855350, surrounding[2]) << error_msg2;
		ref_.ReverseSurrounding(surrounding,0,14);
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 15);
		EXPECT_EQ(613348, surrounding[0]) << error_msg;
		EXPECT_EQ(739384, surrounding[1]) << error_msg;
		EXPECT_EQ(10042, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,0,16);
		ref_.RollBackReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 15);
		EXPECT_EQ(613348, surrounding[0]) << error_msg2;
		EXPECT_EQ(739384, surrounding[1]) << error_msg2;
		EXPECT_EQ(10042, surrounding[2]) << error_msg2;
		ref_.ReverseSurrounding(surrounding,0,496);
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 497);
		EXPECT_EQ(524915, surrounding[0]) << error_msg;
		EXPECT_EQ(720884, surrounding[1]) << error_msg;
		EXPECT_EQ(216468, surrounding[2]) << error_msg;
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 498);
		EXPECT_EQ(917660, surrounding[0]) << error_msg;
		EXPECT_EQ(966653, surrounding[1]) << error_msg;
		EXPECT_EQ(54117, surrounding[2]) << error_msg;
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 499);
		EXPECT_EQ(229415, surrounding[0]) << error_msg;
		EXPECT_EQ(241663, surrounding[1]) << error_msg;
		EXPECT_EQ(275673, surrounding[2]) << error_msg;
		ref_.RollBackReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 498);
		EXPECT_EQ(917660, surrounding[0]) << error_msg2;
		EXPECT_EQ(966653, surrounding[1]) << error_msg2;
		EXPECT_EQ(54117, surrounding[2]) << error_msg2;
		ref_.RollBackReverseSurrounding(surrounding, ref_.ReferenceSequence(0), 497);
		EXPECT_EQ(524915, surrounding[0]) << error_msg2;
		EXPECT_EQ(720884, surrounding[1]) << error_msg2;
		EXPECT_EQ(216468, surrounding[2]) << error_msg2;
		ref_.ReverseSurrounding(surrounding,1,16);
		ref_.UpdateReverseSurrounding(surrounding, ref_.ReferenceSequence(1), 17);
		EXPECT_EQ(960397, surrounding[0]) << error_msg;
		EXPECT_EQ(945044, surrounding[1]) << error_msg;
		EXPECT_EQ(626906, surrounding[2]) << error_msg;
		ref_.ReverseSurrounding(surrounding,1,18);
		ref_.RollBackReverseSurrounding(surrounding, ref_.ReferenceSequence(1), 17);
		EXPECT_EQ(960397, surrounding[0]) << error_msg2;
		EXPECT_EQ(945044, surrounding[1]) << error_msg2;
		EXPECT_EQ(626906, surrounding[2]) << error_msg2;

		error_msg = "The function SumBias returns wrong results\n";
		// cat <(seqtk seq reference-test.fa | awk '(2==NR)') <(seqtk seq -r reference-test.fa | awk '(2==NR)') | awk '{for(i=21;i<=length($0)-50; i+=1){print substr($0,i-10,30), NR, i-1}}' | awk '{print ">"$1, $2, $3; print $1}' | seqtk seq -r | awk '(1==NR%2){full=substr($1,2,length($1)-1); strand=$2-1; pos=$3}(0==NR%2){print strand, pos, substr(full,11,10), full, $0}' | awk '{print $1, $2, gsub(/[GC]/,"",$3), substr($4,1,10), substr($4, 11, 10), substr($4, 21, 10) , substr($5,1,10), substr($5, 11, 10), substr( $5, 21, 10)}' | awk '(0==$1 && (27 == $2 || 50 == $2 || 90 == $2 || 100 == $2 || 162 == $2 || 242 == $2) || 1==$1 && 381==$2)' | awk '{print $1, $2, $3; print $4; print $5; print $6; print $7; print $8; print $9}' | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}{if(NR%7==1){print $0}else{mult=1;sur=0;for(i=length($0);i>0;i-=1){sur+=mult*d[substr($0,i,1)];mult*=4}; print $0, sur}}'
		// cat <(seqtk seq reference-test.fa | awk '(2==NR)') <(seqtk seq -r reference-test.fa | awk '(2==NR)') | awk '{print substr($0,length($0)-9,length($0)) $0 substr($0,1,10)}' | awk '{for(i=11;i<=length($0)-20; i+=1){print substr($0,i-10,30), NR, i-11}}' | awk '{print ">"$1, $2, $3; print $1}' | seqtk seq -r | awk '(1==NR%2){full=substr($1,2,length($1)-1); strand=$2-1; pos=$3}(0==NR%2){print strand, pos, substr(full,11,10), full, $0}' | awk '{print $1, $2, gsub(/[GC]/,"",$3), substr($4,1,10), substr($4, 11, 10), substr($4, 21, 10) , substr($5,1,10), substr($5, 11, 10), substr( $5, 21, 10)}' | awk 'BEGIN{gc[0] = 1.0; gc[3] = 0.1; gc[4] = 0.3; gc[9] = 0.2; base=10; high=base+0.5; normal=base+0.0; low=base-0.5; very_low=base-1; sur["GCAACGGGCA"]=normal; sur["TGGATTAAAA"]=normal; sur["GTTACCTGCC"]=normal; sur["GTGAGTAAAT"]=normal; sur["GCACAGACAG"]=very_low; sur["CCACAGGTAA"]=normal; sur["ATTTAGTGAC"]=high; sur["ATATGTCTCT"]=normal; sur["AAAGAGTGTC"]=normal; sur["GTGAGTAAAT"]=normal; sur["TAAAATTTTA"]=high; sur["ATAAAAATTA"]=normal; sur["CGGTGCGGGC"]=normal; sur["CTAAGTCAAT"]=normal; sur["GTGTGGATTA"]=normal; sur["TGATAGCAGC"]=normal; sur["TAAAATTTTA"]=high; sur["TTGACTTAGG"]=normal; sur["CAGAGTACAC"]=normal; sur["TGACGCGTAC"]=normal; sur["AAAATTTTAA"]=low; sur["TAATCCACAC"]=normal; sur["GCTGCTATCA"]=normal; sur["TAAAATTTTA"]=high; sur["CCTAAGTCAA"]=normal; sur["GTGTACTCTG"]=normal; sur["GTACGCGTCA"]=normal; sur["TTAAAATTTT"]=normal; sur["AGAGACATAT"]=normal; sur["GACACTCTTT"]=normal; sur["ATTTACTCAC"]=normal; sur["TAATTTTTAT"]=normal; sur["GCCCGCACCG"]=normal; sur["ATTGACTTAG"]=normal; sur["TGCCCGTTGC"]=normal; sur["TTTTAATCCA"]=normal; sur["GGCAGGTAAC"]=low; sur["ATTTACTCAC"]=normal; sur["CTGTCTGTGC"]=normal; sur["TTACCTGTGG"]=normal; sur["GTCACTAAAT"]=high}{start=0; for(i=4; i <= 6; i++){if(sur[$i] > 5){start+=sur[$i]-base}else{start-=1000}}; end=0; for(i=7; i <= 9; i++){if(sur[$i] > 5){end+=sur[$i]-base}else{end-=1000}}; print $0, 0.0+gc[$3], start, end}' | awk '{print $0, 2/(1+exp(-$11)), 2/(1+exp(-$12))}' 2>/dev/null | awk '{print $0, $10*$13*$14}' | awk '{print $0, 0.5*$15}' | awk 'BEGIN{sum=0; max=0}{sum += $16; if($16 > max){max=$16}}END{print "sum:", sum, " max: ", max}'
		Vect<double> gc_bias;
		gc_bias[30] = 0.1; // Filtered because it is at ref seq ends for stats creation: So SumBias counts it, GetFragmentSites completely excludes it
		gc_bias[40] = 0.3; // Right at the beginning of the accepted sequence, so should be counted in SumBias and the first one entered in GetFragmentSites
		// gc_bias[30] = 0.1;
		gc_bias[0] = 1.0;
		// gc_bias[0] = 1.0;
		gc_bias[90] = 0.2; // N's will block this one for GetFragmentSites(So placeholder expected)
		// gc_bias[30] = 0.1; // Reverse strand

		array<vector<double>, 3> sur_bias;
		for( auto &block : sur_bias){
			block.resize(Reference::SurroundingSize(), -1000.0); // Set to big negative number, so that bias will be very close to zero
		}

		double val_high = 0.5;
		double val_normal = 0.0;
		double val_low = -0.5;
		double val_very_low = -1.0;
		// Fragment starts
		sur_bias[0][591524] = val_normal; // GCAACGGGCA
		sur_bias[0][954112] = val_normal; // TGGATTAAAA
		sur_bias[0][771557] = val_normal; // GTTACCTGCC
		sur_bias[0][756483] = val_normal; // GTGAGTAAAT
		sur_bias[0][594450] = val_very_low; // GCACAGACAG
		sur_bias[0][332464] = val_normal; // CCACAGGTAA
		sur_bias[0][258785] = val_high; // ATTTAGTGAC
		sur_bias[1][211831] = val_normal; // ATATGTCTCT
		sur_bias[1][8941] = val_normal; // AAAGAGTGTC
		sur_bias[1][756483] = val_normal; // GTGAGTAAAT
		sur_bias[1][787452] = val_high; // TAAAATTTTA
		sur_bias[1][196668] = val_normal; // ATAAAAATTA
		sur_bias[1][440745] = val_normal; // CGGTGCGGGC
		sur_bias[1][461635] = val_normal; // CTAAGTCAAT
		sur_bias[2][768572] = val_normal; // GTGTGGATTA
		sur_bias[2][930377] = val_normal; // TGATAGCAGC
		sur_bias[2][787452] = val_high; // TAAAATTTTA
		sur_bias[2][1017802] = val_normal; // TTGACTTAGG
		sur_bias[2][297745] = val_normal; // CAGAGTACAC
		sur_bias[2][924081] = val_normal; // TGACGCGTAC
		sur_bias[2][4080] = val_low; // AAAATTTTAA

		// Fragment ends
		sur_bias[0][800017] = val_normal; // TAATCCACAC
		sur_bias[0][649012] = val_normal; // GCTGCTATCA
		sur_bias[0][787452] = val_high; // TAAAATTTTA
		sur_bias[0][377552] = val_normal; // CCTAAGTCAA
		sur_bias[0][766430] = val_normal; // GTGTACTCTG
		sur_bias[0][727476] = val_normal; // GTACGCGTCA
		sur_bias[0][983295] = val_normal; // TTAAAATTTT
		sur_bias[1][139571] = val_normal; // AGAGACATAT
		sur_bias[1][542591] = val_normal; // GACACTCTTT
		sur_bias[1][258513] = val_normal; // ATTTACTCAC
		//sur_bias[1][787452] = val_high; // TAAAATTTTA it is also a start surrounding
		sur_bias[1][802803] = val_normal; // TAATTTTTAT
		sur_bias[1][612630] = val_normal; // GCCCGCACCG
		sur_bias[1][254450] = val_normal; // ATTGACTTAG
		sur_bias[2][939769] = val_normal; // TGCCCGTTGC
		sur_bias[2][1044692] = val_normal; // TTTTAATCCA
		sur_bias[2][674497] = val_low; // GGCAGGTAAC
		sur_bias[2][258513] = val_normal; // ATTTACTCAC
		sur_bias[2][505785] = val_normal; // CTGTCTGTGC
		sur_bias[2][989114] = val_normal; // TTACCTGTGG
		sur_bias[2][739075] = val_high; // GTCACTAAAT

		double max_bias(0.0);
		EXPECT_NEAR(2.9367, 2*ref_.SumBias(max_bias, 0, 10, 0.5, gc_bias, sur_bias), 0.0001 ) << error_msg; // The factor of 2 is because SumBias only calculates for one strand
		EXPECT_NEAR(0.774915, max_bias, 0.00001); // Rounding errors during summation using floating point precision in awk gives lower precision than the value stated suggests

		// !!!Introducing N's into reference sequence: Do not call tests that do not support N's afterwards!!!
		ref_.reference_sequences_[0][253] = 'N';
		ref_.reference_sequences_[0][256] = 'N';
		ref_.reference_sequences_[0][263] = 'N';

		// cat reference-test.fa | seqtk seq | awk '(2==NR)' | awk 'BEGIN{split("250,253,254,260",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print i, pos, substr($0,i*10+pos-9,10)}}}' | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}{mult=1;sur=0;for(i=length($3);i>0;i-=1){sur+=mult*d[substr($3,i,1)];mult*=4}; print $1, $2, $3, sur}'
		error_msg = "The function ForwardSurroundingWithN returns wrong results\n";
		array<int32_t, 3> surrounding2;
		ref_.ForwardSurroundingWithN(surrounding2,0,250);
		EXPECT_EQ(27546, surrounding2[0]) << error_msg;
		EXPECT_EQ(-7, surrounding2[1]) << error_msg;
		EXPECT_EQ(-4, surrounding2[2]) << error_msg;
		ref_.ForwardSurroundingWithN(surrounding2,0,260);
		EXPECT_EQ(-7, surrounding2[0]) << error_msg;
		EXPECT_EQ(-4, surrounding2[1]) << error_msg;
		EXPECT_EQ(294914, surrounding2[2]) << error_msg;
		// cat reference-test.fa | seqtk seq | awk '(4==NR)' | awk 'BEGIN{split("250",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print i, pos, substr($0,i*10+pos-9,10)}}}' | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}{mult=1;sur=0;for(i=length($3);i>0;i-=1){sur+=mult*d[substr($3,i,1)];mult*=4}; print $1, $2, $3, sur}'
		ref_.ForwardSurroundingWithN(surrounding2,1,250);
		EXPECT_EQ(315976, surrounding2[0]) << error_msg;
		EXPECT_EQ(222228, surrounding2[1]) << error_msg;
		EXPECT_EQ(636052, surrounding2[2]) << error_msg;
		error_msg = "The function UpdateForwardSurroundingWithN returns wrong results\n";
		ref_.ForwardSurroundingWithN(surrounding2,0,249);
		ref_.UpdateForwardSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), 250, 0);
		EXPECT_EQ(27546, surrounding2[0]) << error_msg;
		EXPECT_EQ(-7, surrounding2[1]) << error_msg;
		EXPECT_EQ(-4, surrounding2[2]) << error_msg;
		for(auto pos=251; pos < 254; ++pos){
			ref_.UpdateForwardSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), pos, 0);
		}
		EXPECT_EQ(714407, surrounding2[0]) << error_msg;
		EXPECT_EQ(-4, surrounding2[1]) << error_msg;
		EXPECT_EQ(-1, surrounding2[2]) << error_msg;
		ref_.UpdateForwardSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), 254, 0);
		EXPECT_EQ(-10, surrounding2[0]) << error_msg;
		EXPECT_EQ(-10, surrounding2[1]) << error_msg;
		EXPECT_EQ(525384, surrounding2[2]) << error_msg;
		for(auto pos=255; pos < 261; ++pos){
			ref_.UpdateForwardSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), pos, 0);
		}
		EXPECT_EQ(-7, surrounding2[0]) << error_msg;
		EXPECT_EQ(-4, surrounding2[1]) << error_msg;
		EXPECT_EQ(294914, surrounding2[2]) << error_msg;
		ref_.ForwardSurroundingWithN(surrounding2,1,249);
		ref_.UpdateForwardSurroundingWithN(surrounding2, ref_.ReferenceSequence(1), 250, 1);
		EXPECT_EQ(315976, surrounding2[0]) << error_msg;
		EXPECT_EQ(222228, surrounding2[1]) << error_msg;
		EXPECT_EQ(636052, surrounding2[2]) << error_msg;
		// cat reference-test.fa | seqtk seq | awk '(2==NR)' | awk 'BEGIN{split("260,262,263,270",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print ">" i, pos; print substr($0,pos+2-i*10,10)}}}' | seqtk seq -r | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}(NR%2==1){pos=substr($0,2,length($0)-1)}(NR%2==0){mult=1;sur=0;for(i=length($0);i>0;i-=1){sur+=mult*d[substr($0,i,1)];mult*=4}; print pos, $0, sur}'
		error_msg = "The function ReverseSurroundingWithN returns wrong results\n";
		ref_.ReverseSurroundingWithN(surrounding2,0,260);
		EXPECT_EQ(-3, surrounding2[0]) << error_msg;
		EXPECT_EQ(-6, surrounding2[1]) << error_msg;
		EXPECT_EQ(353371, surrounding2[2]) << error_msg;
		ref_.ReverseSurroundingWithN(surrounding2,0,270);
		EXPECT_EQ(655351, surrounding2[0]) << error_msg;
		EXPECT_EQ(-3, surrounding2[1]) << error_msg;
		EXPECT_EQ(-6, surrounding2[2]) << error_msg;
		// cat reference-test.fa | seqtk seq | awk '(4==NR)' | awk 'BEGIN{split("250",poslist,",")}{for(p=1;p<=length(poslist);++p){pos=poslist[p];for(i=0;i<3;++i){print ">" i, pos; print substr($0,pos+2-i*10,10)}}}' | seqtk seq -r | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}(NR%2==1){pos=substr($0,2,length($0)-1)}(NR%2==0){mult=1;sur=0;for(i=length($0);i>0;i-=1){sur+=mult*d[substr($0,i,1)];mult*=4}; print pos, $0, sur}'
		ref_.ReverseSurroundingWithN(surrounding2,1,250);
		EXPECT_EQ(503704, surrounding2[0]) << error_msg;
		EXPECT_EQ(1014243, surrounding2[1]) << error_msg;
		EXPECT_EQ(614430, surrounding2[2]) << error_msg;
		error_msg = "The function UpdateReverseSurroundingWithN returns wrong results\n";
		ref_.ReverseSurroundingWithN(surrounding2,0,259);
		ref_.UpdateReverseSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), 260, 0);
		EXPECT_EQ(-3, surrounding2[0]) << error_msg;
		EXPECT_EQ(-6, surrounding2[1]) << error_msg;
		EXPECT_EQ(353371, surrounding2[2]) << error_msg;
		for(auto pos=261; pos < 263; ++pos){
			ref_.UpdateReverseSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), pos, 0);
		}
		EXPECT_EQ(-1, surrounding2[0]) << error_msg;
		EXPECT_EQ(-4, surrounding2[1]) << error_msg;
		EXPECT_EQ(153157, surrounding2[2]) << error_msg;
		ref_.UpdateReverseSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), 263, 0);
		EXPECT_EQ(913149, surrounding2[0]) << error_msg;
		EXPECT_EQ(-10, surrounding2[1]) << error_msg;
		EXPECT_EQ(-10, surrounding2[2]) << error_msg;
		for(auto pos=264; pos < 271; ++pos){
			ref_.UpdateReverseSurroundingWithN(surrounding2, ref_.ReferenceSequence(0), pos, 0);
		}
		EXPECT_EQ(655351, surrounding2[0]) << error_msg;
		EXPECT_EQ(-3, surrounding2[1]) << error_msg;
		EXPECT_EQ(-6, surrounding2[2]) << error_msg;
		ref_.ReverseSurroundingWithN(surrounding2,1,249);
		ref_.UpdateReverseSurroundingWithN(surrounding2, ref_.ReferenceSequence(1), 250, 1);
		EXPECT_EQ(503704, surrounding2[0]) << error_msg;
		EXPECT_EQ(1014243, surrounding2[1]) << error_msg;
		EXPECT_EQ(614430, surrounding2[2]) << error_msg;

		error_msg = "The function GetFragmentSites returns wrong results\n";
		std::vector<FragmentSite> frag_sites;
		ref_.GetFragmentSites(frag_sites, 0, 10, 0, 1000);

		// seqtk seq reference-test.fa | awk '(2==NR){for(i=21;i<=length($0)-50; i+=1){print substr($0,i-10,30), i-1}}' | awk '{print ">"$1, $2; print $1}' | seqtk seq -r | awk '(1==NR%2){full=substr($1,2,length($1)-1); pos=$2}(0==NR%2){print pos, substr(full,11,10), full, $0}' | awk '{print $1, gsub(/[GC]/,"",$2), substr($3,1,10), substr($3, 11, 10), substr($3, 21, 10) , substr($4,1,10), substr($4, 11, 10), substr( $4, 21, 10)}' | awk '(50 == $1 || 100 == $1 || 242 == $1 || 400 == $1)' | awk '{print $1, $2; print $3; print $4; print $5; print $6; print $7; print $8}' | awk 'BEGIN{d["A"]=0;d["C"]=1;d["G"]=2;d["T"]=3}{if(NR%7==1){print $0}else{mult=1;sur=0;for(i=length($0);i>0;i-=1){sur+=mult*d[substr($0,i,1)];mult*=4}; print $0, sur}}'
		EXPECT_EQ(391, frag_sites.size() ) << error_msg; // 500(SequenceLength) -10(fragmentLength)+1 -2*50(min_dist_to_ref_seq_ends_)
		EXPECT_EQ(40, frag_sites[0].gc_) << error_msg;
		EXPECT_EQ(954112, frag_sites[0].start_surrounding_[0]) << error_msg;
		EXPECT_EQ(8941, frag_sites[0].start_surrounding_[1]) << error_msg;
		EXPECT_EQ(930377, frag_sites[0].start_surrounding_[2]) << error_msg;
		EXPECT_EQ(649012, frag_sites[0].end_surrounding_[0]) << error_msg;
		EXPECT_EQ(542591, frag_sites[0].end_surrounding_[1]) << error_msg;
		EXPECT_EQ(1044692, frag_sites[0].end_surrounding_[2]) << error_msg;
		EXPECT_EQ(0, frag_sites[0].count_forward_) << error_msg;
		EXPECT_EQ(0, frag_sites[0].count_reverse_) << error_msg;
		EXPECT_EQ(1.0, frag_sites[0].bias_) << error_msg;
		EXPECT_EQ(0, frag_sites[50].gc_) << error_msg;
		EXPECT_EQ(756483, frag_sites[50].start_surrounding_[0]) << error_msg;
		EXPECT_EQ(787452, frag_sites[50].start_surrounding_[1]) << error_msg;
		EXPECT_EQ(1017802, frag_sites[50].start_surrounding_[2]) << error_msg;
		EXPECT_EQ(377552, frag_sites[50].end_surrounding_[0]) << error_msg;
		EXPECT_EQ(787452, frag_sites[50].end_surrounding_[1]) << error_msg;
		EXPECT_EQ(258513, frag_sites[50].end_surrounding_[2]) << error_msg;
		EXPECT_EQ(0, frag_sites[50].count_forward_) << error_msg;
		EXPECT_EQ(0, frag_sites[50].count_reverse_) << error_msg;
		EXPECT_EQ(1.0, frag_sites[50].bias_) << error_msg;
		EXPECT_EQ(0, frag_sites[192].gc_) << error_msg; // N's are blocking this one (so we just have a placeholder here)
		EXPECT_EQ(0, frag_sites[192].start_surrounding_[0]) << error_msg;
		EXPECT_EQ(0, frag_sites[192].start_surrounding_[1]) << error_msg;
		EXPECT_EQ(0, frag_sites[192].start_surrounding_[2]) << error_msg;
		EXPECT_EQ(0, frag_sites[192].end_surrounding_[0]) << error_msg;
		EXPECT_EQ(0, frag_sites[192].end_surrounding_[1]) << error_msg;
		EXPECT_EQ(0, frag_sites[192].end_surrounding_[2]) << error_msg;
		EXPECT_EQ(0, frag_sites[192].count_forward_) << error_msg;
		EXPECT_EQ(0, frag_sites[192].count_reverse_) << error_msg;
		EXPECT_EQ(0.0, frag_sites[192].bias_) << error_msg;
		EXPECT_EQ(30, frag_sites[350].gc_) << error_msg;
		EXPECT_EQ(454550, frag_sites[350].start_surrounding_[0]) << error_msg;
		EXPECT_EQ(212456, frag_sites[350].start_surrounding_[1]) << error_msg;
		EXPECT_EQ(37093, frag_sites[350].start_surrounding_[2]) << error_msg;
		EXPECT_EQ(675743, frag_sites[350].end_surrounding_[0]) << error_msg;
		EXPECT_EQ(870451, frag_sites[350].end_surrounding_[1]) << error_msg;
		EXPECT_EQ(430150, frag_sites[350].end_surrounding_[2]) << error_msg;
		EXPECT_EQ(0, frag_sites[350].count_forward_) << error_msg;
		EXPECT_EQ(0, frag_sites[350].count_reverse_) << error_msg;
		EXPECT_EQ(1.0, frag_sites[350].bias_) << error_msg;

		// Test ReplaceN
		for(uint32_t i=50; i<150; ++i){
			ref_.reference_sequences_[0][i] = 'N';
			ref_.reference_sequences_[1][i+300] = 'N';
		}
		uint32_t n_in_reference(203); // 3 from further up where we included them to test surroundings
		array< uint64_t, 4 > old_values = {{0,0,0,0}};
		for( const auto &seq : ref_.reference_sequences_ ){
			for( auto base : seq ){
				if( static_cast<uint16_t>(base) < 4 ){
					old_values.at( static_cast<uint16_t>(base) ) += 1;
				}
			}
		}

		ref_.ReplaceN(317);
		array< uint64_t, 4 > new_values = {{0,0,0,0}};
		for( const auto &seq : ref_.reference_sequences_ ){
			for( auto base : seq ){
				if( static_cast<uint16_t>(base) < 4 ){
					new_values.at( static_cast<uint16_t>(base) ) += 1;
				}
			}
		}

		EXPECT_EQ(old_values.at(0)+old_values.at(1)+old_values.at(2)+old_values.at(3)+n_in_reference, new_values.at(0)+new_values.at(1)+new_values.at(2)+new_values.at(3)) << "Not all N's properly replaced\n";
		for(uint16_t i=4; i--; ){
			EXPECT_LT(old_values.at(i), new_values.at(i)) << "No N's converted to " << i << '\n';
		}
	}

	TEST_F(ReferenceTest, GZip){
		ASSERT_TRUE( ref_.ReadFasta( (string(PROJECT_SOURCE_DIR)+"/test/reference-test.fa.gz").c_str() ) );

		ASSERT_EQ(2, ref_.NumberSequences()) << "The gz test file did not load properly\n";
	}

	TEST_F(ReferenceTest, BZip2){
		ASSERT_TRUE( ref_.ReadFasta( (string(PROJECT_SOURCE_DIR)+"/test/reference-test.fa.bz2").c_str() ) );

		ASSERT_EQ(2, ref_.NumberSequences()) << "The bz2 test file did not load properly\n";
	}
}