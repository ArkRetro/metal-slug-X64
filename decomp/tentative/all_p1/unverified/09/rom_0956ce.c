/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0956ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_956ce(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_4256();
  candidate_9ea8();
  iVar2 = unaff_A6[3];
  *(ushort *)((int)unaff_A6 + 0x26) = *(byte *)(iVar2 + 0x94) + 0x100;
  *(undefined1 *)((int)unaff_A6 + 0x75) = *(undefined1 *)(iVar2 + 0x75);
  *(undefined1 *)(unaff_A6 + 0x1d) = *(undefined1 *)(iVar2 + 0x74);
  if (*(int *)(iVar2 + 0x30) < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    *(char *)((int)unaff_A6 + 0x75) = '\x04' - *(char *)((int)unaff_A6 + 0x75);
  }
  candidate_1ab98();
  *unaff_A6 = 0x95768;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar4) {
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
