/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0824a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_824a2(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  undefined4 uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  candidate_4256();
  uVar4 = 0xc000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar4 = 0xffff4000;
  }
  unaff_A6[0xc] = uVar4;
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar4;
  *unaff_A6 = 0x824d2;
  uVar5 = false;
  candidate_82af6();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x7d) = 1;
    *unaff_A6 = candidate_82514;
    uVar5 = 0;
  }
  candidate_427a();
  candidate_82b4e();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_82514;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x7d) = 1;
    *unaff_A6 = candidate_82514;
  }
  uVar5 = false;
  if (0 < *(short *)(unaff_A6 + 0x1d)) {
    sVar3 = *(short *)(unaff_A6 + 0x1d);
    uVar5 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x1d) = sVar3 + -1;
  }
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_826c2;
    uVar5 = 0;
  }
  candidate_1945c();
  if (!(bool)uVar5) {
    return;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
