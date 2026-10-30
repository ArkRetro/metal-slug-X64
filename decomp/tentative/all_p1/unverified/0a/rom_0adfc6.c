/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0adfc6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_adfc6(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  func_0x000adeb6();
  *(undefined1 *)((int)unaff_A6 + 0x7e) = *(undefined1 *)(unaff_A6[3] + 0x7e);
  *(undefined1 *)(unaff_A6 + 0x20) = *(undefined1 *)(unaff_A6[3] + 0x80);
  if (*(char *)(unaff_A6[3] + 0x94) == '\0') {
    candidate_ade96();
    candidate_1abc8();
    iVar2 = (uint)*(byte *)((int)unaff_A6 + 0x7e) * 4;
    *(short *)(unaff_A6 + 0x10) = *(short *)(iVar2 + 0xae026) + *(short *)(unaff_A6 + 0x10);
    candidate_427a();
    *(short *)(unaff_A6 + 0xf) = *(short *)(unaff_A6 + 0xf) + 6;
    *(short *)(unaff_A6 + 0x10) = *(short *)(iVar2 + 0xae028) + *(short *)(unaff_A6 + 0x10);
    *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
    candidate_427a();
    return;
  }
  candidate_4256();
  *unaff_A6 = candidate_adf40;
  uVar3 = 0;
  candidate_ade96();
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
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
