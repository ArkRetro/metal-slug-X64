/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085294.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_85294(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_8536c();
  candidate_85cc0();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  unaff_A6[0x14] = 0xe4e20;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0x28] =
         *(undefined4 *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x8a) << 2) + 0xe4d04);
    *(undefined2 *)(unaff_A6 + 0x29) = 0;
    *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  }
  else {
    unaff_A6[0x28] =
         *(undefined4 *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x8a) << 2) + 0xe4d10);
    *(undefined2 *)(unaff_A6 + 0x29) = 0;
    *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  }
  *unaff_A6 = 0x85306;
  uVar2 = 0;
  candidate_21288();
  candidate_427a();
  candidate_85484();
  candidate_4c24();
  if ((bool)uVar2) {
    *(byte *)((int)unaff_A6 + 0x87) = *(byte *)((int)unaff_A6 + 0x87) | 6;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_8535e;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_85354;
  }
  if (*(char *)((int)unaff_A6 + 0x87) != '\0') {
    *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + -1;
  }
  return;
}
