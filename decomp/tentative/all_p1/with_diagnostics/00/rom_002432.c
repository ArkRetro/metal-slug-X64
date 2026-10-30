/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002432.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2432(void)

{
  undefined4 *puVar1;

  thunk_FUN_00001070();
  DAT_0010fd80 = DAT_0010fd80 & 0x7f;
  _DAT_003c000c = 4;
  uRam00100014 = 1;
  _DAT_002ffff0 = 1;
  do {
  } while (DAT_002fffff != '\x01');
  if (DAT_0010fdae == '\0') {
    DAT_0010fdaf = 0;
    DAT_0010f002 = 0;
    DAT_0010f004 = 0;
    _DAT_0010f000 = 0;
    candidate_57b28();
    if (DAT_0010fd82 == '\0') {
      candidate_3596();
      candidate_ca6a0();
      candidate_ca812();
    }
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  _DAT_00100012 = 0;
  _DAT_003c0006 = 0;
  _DAT_00100010 = 0x8200;
  _DAT_003c0004 = 0;
  candidate_2852();
  DAT_00100002 = -((_DAT_003c0006 & 8) != 0);
  DAT_0010f005 = 0;
  _DAT_00100004 = 0x505;
  candidate_240e();
  candidate_2382();
  candidate_23be();
  DAT_0010fdb6 = 0;
  DAT_0010fdb7 = 0;
  DAT_0010fdb8 = 0;
  DAT_0010fdb9 = 0;
  if (DAT_0010fdae == '\x01') {
    DAT_0010f003 = 0;
    DAT_0010fdaf = 0;
  }
  else if (DAT_0010fdae == '\x02') {
    DAT_0010f003 = 0;
    DAT_0010fdaf = 1;
  }
  else {
    if (DAT_0010fdae != '\x03') {
      DAT_0010fdb6 = 0;
      DAT_0010fdb7 = 0;
      DAT_0010fdb8 = 0;
      DAT_0010fdb9 = 0;
      halt_baddata();
    }
    DAT_0010fec5 = 1;
    DAT_0010fdaf = 1;
  }
  if (DAT_0010fe80 == '\0') {
    _DAT_0010f000 = 0;
  }
  DAT_00300001 = DAT_0010fdae;
  DAT_00300001 = candidate_3b6a();
  DAT_00300001 = candidate_3ca74();
  DAT_00300001 = candidate_1aac2();
  DAT_00300001 = candidate_5152();
  DAT_00300001 = candidate_3f9e();
  DAT_00300001 = candidate_1205e();
  DAT_00300001 = candidate_3668();
  if (DAT_0010fd82 != '\0') {
    candidate_ca5b0();
  }
  DAT_0010c2e7 = 0;
  DAT_0010fd80 = DAT_0010fd80 | 0x80;
  if (DAT_0010fdae != '\x01') {
    puVar1 = (undefined4 *)0x1010a4;
    _DAT_001063dc = (undefined1 *)register0x0000003c;
    do {
      (*(code *)*puVar1)();
      puVar1 = (undefined4 *)puVar1[2];
    } while( true );
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}
