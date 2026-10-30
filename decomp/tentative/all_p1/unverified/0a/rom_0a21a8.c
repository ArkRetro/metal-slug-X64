/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a21a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a21a8(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  undefined4 uVar4;
  undefined4 extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_12022();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
  candidate_1ad8c();
  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  uVar4 = 0x22df54;
  if (*(short *)((int)unaff_A6 + 0x82) < 0) {
    uVar3 = candidate_29c6();
    *(ushort *)((int)unaff_A6 + 0x82) = uVar3 & 0xf;
    uVar4 = extraout_A1;
  }
  *(undefined4 *)((int)unaff_A6 + 0x6e) = uVar4;
  candidate_4256();
  candidate_19240();
  candidate_9ea8();
  *unaff_A6 = 0xa2240;
  if (*(char *)(unaff_A6[3] + 0x91) != '\0') {
    *(char *)((int)unaff_A6 + 0x91) = *(char *)(unaff_A6[3] + 0x91);
  }
  candidate_1abc8();
  bVar5 = false;
  if (*(short *)((int)unaff_A6 + 0x82) < 0) {
    candidate_427a();
    if (bVar5) {
      if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
        candidate_4126();
        iVar2 = *(int *)(unaff_A5 + -0x1c24);
        do {
          unaff_A6 = (undefined4 *)unaff_A6[2];
          pcVar1 = (code *)*unaff_A6;
          *(undefined4 *)(iVar2 + -4) = 0x424a;
          (*pcVar1)();
        } while( true );
      }
      candidate_4256();
    }
  }
  else {
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  }
  return;
}
