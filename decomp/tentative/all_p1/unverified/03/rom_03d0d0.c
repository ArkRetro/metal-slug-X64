/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d0d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3d0d0(void)

{
  short sVar1;
  char unaff_D3b;
  ushort unaff_D4w;
  ushort unaff_D7w;
  undefined4 in_A0;
  code *extraout_A1;
  code *pcVar2;

  pcVar2 = candidate_3cf9a;
  do {
    if (unaff_D4w == 1) {
      unaff_D3b = -1;
    }
    sVar1 = 0;
    if (unaff_D4w < 6) {
      sVar1 = (short)(((uint)unaff_D7w / (uint)*(ushort *)((short)(unaff_D4w * 2) + 0x3d0c4)) % 10);
    }
    if (sVar1 == 0) {
      if (unaff_D3b != '\0') goto LAB_0003d114;
    }
    else {
      unaff_D3b = -1;
LAB_0003d114:
      (*pcVar2)(in_A0);
      pcVar2 = extraout_A1;
    }
    unaff_D4w = unaff_D4w - 1;
    if (unaff_D4w == 0) {
      return;
    }
  } while( true );
}
