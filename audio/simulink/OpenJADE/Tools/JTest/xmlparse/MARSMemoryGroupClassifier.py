#*********************************************************************************
# This script is classifier for memory group
#
#*********************************************************************************
class MARSMemoryGroupClassifier:

    g_datasection = [
        'dxe_block1_data_awe_prio0_bw',
        'dxe_block2_awe_data_prio0_bw',
        'dxe_block2_awe_data_prio0',
        'dxe_l2_uncached_c1_bw',
        'dxe_l2_uncached_c1',
        'dxe_l2_uncached_c2_bw',
        'dxe_l2_uncached_c2',
        'dxe_block1_l1_dmda_bw',
        'dxe_block2_l1_pmda_bw',
        'dxe_block0_l1_data_bw',
        'dxe_block1_l1_data_bw',
        'dxe_block0_data_dm',
        'dxe_block0_data_prio0',
        'dxe_block0_data_prio0_bw',
        'dxe_block0_data_prio1',
        'dxe_block0_data_prio1_bw',
        'dxe_block0_data_prio2',
        'dxe_block0_data_prio2_bw',
        'dxe_block0_data_prio3',
        'dxe_block0_data_prio3_bw',
        'dxe_block1_data_prio0',
        'dxe_block1_data_prio0_bw',
        'dxe_block1_data_prio1',
        'dxe_block1_data_prio1_bw',
        'dxe_block1_data_prio2',
        'dxe_block1_data_prio2_bw',
        'dxe_block1_data_prio3',
        'dxe_block1_data_prio3_bw',
        'dxe_block2_data_prio0',
        'dxe_block2_data_prio0_bw',
        'dxe_block2_pm_data_prio1',
        'dxe_block2_pm_data_prio1_bw',
        'dxe_block2_pm_data_prio2',
        'dxe_block2_pm_data_prio2_bw',
        'dxe_block2_dm_data_prio2_bw',
        'dxe_block2_pm_data_prio3',
        'dxe_block2_pm_data_prio3_bw',
        'dxe_block3_data_prio0',
        'dxe_block3_data_prio0_bw',
        'dxe_block2_data',
        'dxe_block2_data_bw',
        'dxe_block0_pm_data',
        'dxe_block0_pm_data_bw',
        'dxe_block1_pm_data',
        'dxe_block1_pm_data_bw',
        'dxe_block3_data',
        'dxe_block3_data_bw',
        'dxe_block3_pm_data',
        'dxe_block3_pm_data_bw',
        'dxe_l2_executable_name',
        'dxe_l2_cpp_ctors_nw',
        'dxe_l2_cpp_ctors_bw',
        'dxe_l2_data',
        'dxe_l2_data_bw',
        'seg_l2_shared_bw',
        'dxe_l2_pm_data',
        'dxe_l2_pm_data_bw',
        'dxe_l2_uncached',
        'dxe_l2_uncached_bw',
        'dxe_sdram_cpp_ctors_nw',
        'dxe_sdram_cpp_ctors_bw',
        'dxe_sdram_data',
        'dxe_sdram_data_bw',
        'dxe_sdram_A_data',
        'dxe_sdram_A_data_bw',
        'dxe_sdram_A_pm_data',
        'dxe_sdram_A_pm_data_bw',
        'dxe_sdram_B_data',
        'dxe_sdram_B_data_bw',
        'dxe_sdram_B_pm_data',
        'dxe_sdram_B_pm_data_bw',
        'dxe_block1_mydata_prio0_bw',
        'dxe_block2_mydata_prio0_bw',
                ]

    g_bsssection = [
        'dxe_l2_awe_noinit',
        'dxe_l2_awe_noinit_bw',
        'dxe_l2_awe_noinit',
        'dxe_sdram_awe_noinit',
        'dxe_sdram_awe_noinit_bw',
        'dxe_sdram_bsz_bw',
        'dxe_block0_noinit_dm',
        'dxe_block0_bsz_dm',
        'dxe_block0_noinit_prio0',
        'dxe_block0_noinit_prio0_bw',
        'dxe_block0_noinit_prio1',
        'dxe_block0_noinit_prio1_bw',
        'dxe_block0_noinit_prio2',
        'dxe_block0_noinit_prio2_bw',
        'dxe_block0_noinit_prio3',
        'dxe_block0_noinit_prio3_bw',
        'dxe_block1_noinit_prio0',
        'dxe_block1_noinit_prio0_bw',
        'dxe_block1_noinit_prio1',
        'dxe_block1_noinit_prio1_bw',
        'dxe_block1_noinit_prio2',
        'dxe_block1_noinit_prio2_bw',
        'dxe_block1_noinit_prio3',
        'dxe_block1_noinit_prio3_bw',
        'dxe_block2_noinit_prio0',
        'dxe_block2_noinit_prio0_bw',
        'dxe_block3_noinit_prio0',
        'dxe_block3_noinit_prio0_bw',
        'dxe_block0_bsz_prio0',
        'dxe_block0_bsz_prio0_bw',
        'dxe_block0_bsz_prio1',
        'dxe_block0_bsz_prio1_bw',
        'dxe_block0_bsz_prio2',
        'dxe_block0_bsz_prio2_bw',
        'dxe_block0_bsz_prio3',
        'dxe_block0_bsz_prio3_bw',
        'dxe_block1_bsz_prio0',
        'dxe_block1_bsz_prio0_bw',
        'dxe_block1_bsz_prio1',
        'dxe_block1_bsz_prio1_bw',
        'dxe_block1_bsz_prio2',
        'dxe_block1_bsz_prio2_bw',
        'dxe_block1_bsz_prio3',
        'dxe_block1_bsz_prio3_bw',
        'dxe_block2_bsz_prio0',
        'dxe_block2_bsz_prio0_bw',
        'dxe_block3_bsz_prio0',
        'dxe_block3_bsz_prio0_bw',
        'dxe_block2_bsz',
        'dxe_block2_bsz_bw',
        'dxe_block3_bsz',
        'dxe_block3_bsz_bw',
        'dxe_l2_bsz',
        'dxe_l2_bsz_bw',
        'dxe_l2_noinit',
        'dxe_l2_noinit_bw',
        'dxe_sdram_noinit',
        'dxe_sdram_noinit_bw',
        'dxe_sdram_A_bsz',
        'dxe_sdram_A_bsz_bw',
        'dxe_sdram_B_bsz',
        'dxe_sdram_B_bsz_bw',
        'dxe_block2_noinit',
        'dxe_block2_noinit_bw',
        'dxe_block3_noinit',
        'dxe_block3_noinit_bw',
        'dxe_l2bootcode_noinit',
        'dxe_l2bootcode_noinit_bw',
        'dxe_sdram_A_noinit_bw']

    g_textsection = [
        'dxe_block3_sw_code_awe_prio0',
        'dxe_block3_nw_code_awe_prio0',
        'dxe_l2_awe_sw_code',
        'dxe_l2_awe_nw_code',
        'dxe_iv_code',
        'dxe_block0_sw_code_prio0',
        'dxe_block0_nw_code_prio0',
        'dxe_block0_sw_code',
        'dxe_block0_nw_code',
        'dxe_block2_nw_code',
        'dxe_block1_sw_code_prio0',
        'dxe_block1_nw_code_prio0',
        'dxe_block2_sw_code_prio0',
        'dxe_block2_nw_code_prio0',
        'dxe_block3_nw_code_prio0',
        'dxe_block3_sw_code_prio0',
        'dxe_block3_sw_code_prio1',
        'dxe_block3_nw_code_prio1',
        'dxe_block3_sw_code_prio2',
        'dxe_block3_nw_code_prio2',
        'dxe_block3_sw_code_prio3',
        'dxe_block3_nw_code_prio3',
        'dxe_sdram_sw_code',
        'dxe_l2_sw_code',
        'dxe_l2_nw_code',
        'dxe_block2_sw_code_prio2',
        'dxe_l2_mysw_code'
        ]

    g_memory_groups = [
        'data',
        'bss',
        'text',
        'Heap',
        'Stack']

    def groups(self):
        return self.g_memory_groups

    def classify(self, section_name):
        if section_name in self.g_datasection:
            return 'data'
        elif section_name in self.g_bsssection:
            return 'bss'
        elif section_name in self.g_textsection:
            return 'text'
        elif section_name == 'Heap':
            return 'Heap'
        elif section_name == 'Stack':
            return 'Stack'
        else:
            return None