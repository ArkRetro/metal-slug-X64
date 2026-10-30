/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be27e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be27e(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  unaff_A6[0x14] = 0x2d4098;
  candidate_4256();
  unaff_A6[0x1f] = 0x2a95a6;
  unaff_A6[0x20] = 0xbe560;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 2;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 2;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0x560;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0x860;
  candidate_1abc8();
  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x70) = *(undefined2 *)(unaff_A6 + 0x1c);
  *(undefined2 *)(extraout_A0 + 0x74) = *(undefined2 *)(unaff_A6 + 0x1d);
  candidate_1acf2();
  candidate_40bc();
  *(undefined2 *)(extraout_A0_00 + 0x70) = *(undefined2 *)(unaff_A6 + 0x1c);
  *(undefined2 *)(extraout_A0_00 + 0x74) = *(undefined2 *)(unaff_A6 + 0x1d);
  candidate_1acf2();
  unaff_A6[0x1e] = 0xbdeea;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  *unaff_A6 = 0xbe38a;
  uVar4 = 0;
  candidate_1abc8();
  candidate_be446();
  if ((bool)uVar4) {
    return;
  }
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar4) {
    (*(code *)unaff_A6[0x20])();
    *unaff_A6 = candidate_be3c0;
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 1;
  }
  if (*(ushort *)(unaff_A6 + 0x1d) <= *(ushort *)(unaff_A5 + 0x699e)) {
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
