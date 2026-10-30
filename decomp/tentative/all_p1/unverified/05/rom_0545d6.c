/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0545d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_545d6(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_120f8();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  candidate_4256();
  *unaff_A6 = 0x545fa;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  candidate_1ac38();
  if (0x1c0000 < (int)unaff_A6[0xb]) {
    unaff_A6[0xb] = 0x1c0000;
  }
  candidate_427a();
  if (-0x94 < *(short *)(unaff_A6 + 0xb)) {
    return;
  }
  unaff_A6[0xb] = 0x1c0000;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x28;
  *unaff_A6 = 0x54642;
  candidate_1abc8();
  sVar3 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar3;
  if (-1 < sVar3) {
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x3c;
  candidate_19240();
  candidate_9ea8();
  candidate_4256();
  candidate_12022();
  *unaff_A6 = 0x54684;
  candidate_1abc8();
  sVar3 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar3;
  if (sVar3 < 0) {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  uVar4 = 0;
  if ((*(ushort *)((int)unaff_A6 + 0x7a) & 3) == 0) {
    uVar4 = false;
    candidate_40bc();
    if ((bool)uVar4) {
      candidate_1b220();
      uVar4 = 1;
    }
  }
  candidate_427a();
  if ((bool)uVar4) {
    candidate_4256();
  }
  return;
}
