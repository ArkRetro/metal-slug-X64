/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01fda4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1fda4(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_13f1e();
  if (unaff_A6 == *(undefined4 **)(unaff_A6[3] + 0x82)) {
    *(undefined4 *)(unaff_A6[3] + 0x82) = 0;
  }
  if (*(short *)((int)unaff_A6 + 0x72) != 0) {
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
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 2;
  candidate_1b04a();
  candidate_1ab98();
  candidate_1ad52();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = -unaff_A6[0xd];
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  *unaff_A6 = 0x1fe30;
  candidate_1ac38();
  candidate_204de();
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = candidate_1fece;
  }
  bVar4 = false;
  candidate_4c24();
  if (bVar4) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      *unaff_A6 = candidate_1fece;
    }
  }
  bVar4 = false;
  uVar5 = 0;
  candidate_1495c();
  if (!bVar4) {
    candidate_c8022();
    *unaff_A6 = candidate_1fece;
    uVar5 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_1fece;
    uVar5 = 0;
  }
  candidate_1945c();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_2010e;
  }
  return;
}
