/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b9286.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b9286(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  if (*(ushort *)(unaff_A5 + 0x69d0) <= *(ushort *)(unaff_A5 + 0x699e)) {
    candidate_40bc();
  }
  candidate_40bc();
  *(undefined4 *)(extraout_A0 + 0x28) = 0x10600000;
  *(undefined4 *)(extraout_A0 + 0x2c) = 0x300000;
  *(undefined4 *)(extraout_A0 + 0x38) = 0x10e968;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  unaff_A6[0xc] = 0xffff0000;
  DAT_0010c6bd = 0;
  *unaff_A6 = 0xb930e;
  candidate_1ac38();
  if (0xf0f < *(ushort *)(unaff_A5 + 0x699e)) {
    unaff_A6[0xc] = 0x10000;
  }
  if ((int)unaff_A6[10] < 0xf600000) {
    unaff_A6[0xc] = 0x10000;
  }
  if (0xf9fffff < (int)unaff_A6[10]) {
    if (*(ushort *)(unaff_A5 + 0x699e) < 0xf10) {
      unaff_A6[0xc] = 0xffff0000;
    }
    else {
      unaff_A6[10] = 0xfa00000;
      *unaff_A6 = candidate_b939a;
      *(undefined1 *)(unaff_A5 + 0x46bd) = 1;
    }
  }
  candidate_140c0();
  candidate_427a();
  if (0x10ef < *(ushort *)(unaff_A5 + 0x699e)) {
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
