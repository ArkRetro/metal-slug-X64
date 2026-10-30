/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038d1e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38d1e(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  candidate_39502();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  candidate_40bc();
  candidate_1b220();
  *(short *)(extraout_A0 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) * 2;
  candidate_40bc();
  candidate_1b220();
  *(short *)(extraout_A0_00 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) * 2 + 1;
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  *(undefined1 *)(unaff_A6 + 0x1d) = 0;
  *unaff_A6 = 0x38ec6;
  if (*(char *)(unaff_A6 + 0x1d) != '\0') {
    candidate_bc548();
    *unaff_A6 = 0x38eda;
  }
  if ((*(byte *)(unaff_A6 + 0x1d) & 2) != 0) {
    candidate_4256();
    *unaff_A6 = 0x38efa;
  }
  candidate_1abc8();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0xf) < -0x5f) {
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if ((*(short *)((int)unaff_A6 + 0x6e) == 1) && (*(char *)(unaff_A6 + 0x1d) == '\0')) {
    candidate_bc542();
  }
  return;
}
