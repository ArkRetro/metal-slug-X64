/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079fd8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79fd8(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  func_0x000143e4();
  if (*(short *)(unaff_A6 + 0x20) == 10) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x20) == 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  sVar3 = *(short *)(unaff_A6 + 0x20);
  *(short *)(unaff_A6 + 0x20) = sVar3 + -1;
  bVar4 = (*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0;
  if (!bVar4) {
    *unaff_A6 = candidate_4188;
  }
  bVar4 = bVar4 && sVar3 == 0;
  candidate_7a032();
  if (bVar4) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
