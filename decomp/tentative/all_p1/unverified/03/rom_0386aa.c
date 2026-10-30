/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0386aa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_386aa(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  candidate_1abc8();
  uVar4 = *(short *)(unaff_A6[3] + 0x28) - *(short *)(unaff_A6 + 10);
  if ((short)uVar4 < 0) {
    uVar4 = -uVar4;
  }
  if (uVar4 < 0x50) {
    return;
  }
  *unaff_A6 = 0x386d2;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    sVar3 = *(short *)((int)unaff_A6 + 0x8a) + -1;
    *(short *)((int)unaff_A6 + 0x8a) = sVar3;
    if (sVar3 < 0) {
      candidate_38228();
      candidate_38148();
    }
    candidate_3853a();
    candidate_37512();
    candidate_427a();
    return;
  }
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x37ab4;
  uVar5 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
