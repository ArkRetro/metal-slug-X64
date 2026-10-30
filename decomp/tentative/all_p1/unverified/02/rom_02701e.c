/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02701e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2701e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x28;
  candidate_1abc8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  if ((_DAT_0010000e & 1) == 0) {
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      candidate_1b220();
    }
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      candidate_1b220();
    }
  }
  else {
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      candidate_1b220();
    }
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      candidate_1b220();
    }
  }
  if (*(short *)(unaff_A6 + 0x10) < 0x221) {
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
