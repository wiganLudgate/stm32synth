/*
 * wavetable.c
 *
 *	Holds data for waveforms
 *
 *  Created on: 16 juli 2020
 *      Author: Mikael Hessel
 */

#include "modules/wavetable.h"


// SINE TABLE, 2048 positions, 2047 values
const float sinetable[] = {
		0.f, 0.0030694555f, 0.00613888213f, 0.00920825079f, 0.0122775333f, 0.0153466994f, 0.0184157211f,
		0.0214845687f, 0.0245532151f, 0.0276216306f, 0.0306897834f, 0.0337576494f, 0.036825195f, 0.0398923978f,
		0.0429592207f, 0.0460256413f, 0.049091626f, 0.0521571487f, 0.0552221835f, 0.0582866929f, 0.0613506548f,
		0.0644140393f, 0.0674768165f, 0.0705389604f, 0.0736004412f, 0.0766612217f, 0.0797212869f, 0.0827805921f,
		0.0858391225f, 0.0888968483f, 0.0919537321f, 0.0950097516f, 0.0980648696f, 0.101119071f, 0.104172319f,
		0.107224584f, 0.110275835f, 0.11332605f, 0.1163752f, 0.119423248f, 0.122470178f, 0.125515953f,
		0.128560543f, 0.131603912f, 0.134646058f, 0.137686923f, 0.140726492f, 0.143764749f, 0.146801636f,
		0.149837151f, 0.152871251f, 0.155903906f, 0.158935085f, 0.161964789f, 0.164992943f, 0.168019563f,
		0.171044588f, 0.174068004f, 0.177089781f, 0.180109888f, 0.183128297f, 0.186144978f, 0.189159915f,
		0.192173064f, 0.19518441f, 0.198193908f, 0.201201528f, 0.204207271f, 0.207211092f, 0.210212946f,
		0.213212833f, 0.216210693f, 0.219206527f, 0.222200304f, 0.225191981f, 0.228181526f, 0.231168941f,
		0.234154165f, 0.237137184f, 0.240117967f, 0.243096486f, 0.246072724f, 0.249046639f, 0.252018213f,
		0.254987389f, 0.25795418f, 0.260918558f, 0.263880461f, 0.266839862f, 0.269796789f, 0.272751123f,
		0.275702924f, 0.278652132f, 0.281598687f, 0.28454259f, 0.287483841f, 0.29042235f, 0.293358147f,
		0.296291173f, 0.299221426f, 0.302148819f, 0.30507341f, 0.307995081f, 0.310913891f, 0.31382975f,
		0.316742659f, 0.319652587f, 0.322559476f, 0.325463355f, 0.328364164f, 0.331261873f, 0.334156454f,
		0.337047905f, 0.339936167f, 0.34282124f, 0.345703065f, 0.348581642f, 0.35145694f, 0.35432893f,
		0.357197553f, 0.360062838f, 0.362924725f, 0.365783185f, 0.368638217f, 0.371489763f, 0.374337792f,
		0.377182305f, 0.380023271f, 0.382860661f, 0.385694444f, 0.388524592f, 0.391351074f, 0.394173861f,
		0.396992952f, 0.399808288f, 0.402619869f, 0.405427665f, 0.408231616f, 0.411031723f, 0.413827986f,
		0.416620314f, 0.419408739f, 0.422193199f, 0.424973696f, 0.4277502f, 0.43052265f, 0.433291048f,
		0.436055362f, 0.438815564f, 0.441571653f, 0.44432357f, 0.447071284f, 0.449814826f, 0.452554077f,
		0.455289096f, 0.458019823f, 0.460746229f, 0.463468313f, 0.466186017f, 0.468899339f, 0.471608222f,
		0.474312663f, 0.477012634f, 0.479708135f, 0.482399106f, 0.485085517f, 0.487767369f, 0.49044463f,
		0.493117273f, 0.495785266f, 0.498448581f, 0.501107216f, 0.503761113f, 0.506410241f, 0.509054661f,
		0.511694193f, 0.514329016f, 0.516958892f, 0.519583941f, 0.522204101f, 0.524819374f, 0.52742964f,
		0.530034959f, 0.532635272f, 0.535230577f, 0.537820876f, 0.540406048f, 0.542986155f, 0.545561135f,
		0.548130989f, 0.550695717f, 0.5532552f, 0.555809498f, 0.55835855f, 0.560902297f, 0.5634408f,
		0.565973997f, 0.56850189f, 0.571024418f, 0.573541522f, 0.576053262f, 0.578559518f, 0.58106041f,
		0.583555758f, 0.586045623f, 0.588530004f, 0.591008782f, 0.593482018f, 0.59594965f, 0.598411739f,
		0.600868106f, 0.60331881f, 0.605763912f, 0.608203232f, 0.61063683f, 0.613064706f, 0.615486801f,
		0.617903054f, 0.620313525f, 0.622718155f, 0.625116885f, 0.627509773f, 0.6298967f, 0.632277727f,
		0.634652793f, 0.63702184f, 0.639384925f, 0.641741991f, 0.644092977f, 0.646437943f, 0.64877677f,
		0.651109517f, 0.653436124f, 0.655756533f, 0.658070803f, 0.660378873f, 0.662680745f, 0.664976299f,
		0.667265654f, 0.66954869f, 0.671825469f, 0.674095869f, 0.676359892f, 0.678617597f, 0.680868864f,
		0.683113754f, 0.685352206f, 0.687584221f, 0.68980968f, 0.692028701f, 0.694241226f, 0.696447134f,
		0.698646545f, 0.700839341f, 0.70302552f, 0.705205083f, 0.70737803f, 0.709544301f, 0.711703897f,
		0.713856757f, 0.716002941f, 0.718142331f, 0.720274985f, 0.722400844f, 0.724519849f, 0.726632118f,
		0.728737473f, 0.730835974f, 0.73292762f, 0.735012293f, 0.737090111f, 0.739160955f, 0.741224825f,
		0.743281722f, 0.745331645f, 0.747374535f, 0.749410331f, 0.751439154f, 0.753460824f, 0.755475402f,
		0.757482886f, 0.759483218f, 0.761476398f, 0.763462424f, 0.765441239f, 0.767412841f, 0.769377232f,
		0.77133435f, 0.773284197f, 0.775226772f, 0.777162075f, 0.779090047f, 0.781010628f, 0.782923877f,
		0.784829736f, 0.786728203f, 0.78861928f, 0.790502906f, 0.792379141f, 0.794247866f, 0.79610908f,
		0.797962844f, 0.799809039f, 0.801647723f, 0.803478897f, 0.805302441f, 0.807118416f, 0.808926761f,
		0.810727537f, 0.812520623f, 0.81430608f, 0.816083908f, 0.817853987f, 0.819616377f, 0.821371078f,
		0.823117971f, 0.824857175f, 0.826588571f, 0.828312159f, 0.830027997f, 0.831735969f, 0.833436131f,
		0.835128427f, 0.836812854f, 0.838489413f, 0.840158105f, 0.84181881f, 0.843471587f, 0.845116496f,
		0.846753359f, 0.848382294f, 0.850003183f, 0.851616144f, 0.853220999f, 0.854817867f, 0.856406629f,
		0.857987404f, 0.859560013f, 0.861124575f, 0.862681031f, 0.864229321f, 0.865769506f, 0.867301524f,
		0.868825316f, 0.870341003f, 0.871848404f, 0.8733477f, 0.87483871f, 0.876321495f, 0.877795935f,
		0.879262209f, 0.880720139f, 0.882169843f, 0.883611143f, 0.885044217f, 0.886468887f, 0.887885213f,
		0.889293194f, 0.89069277f, 0.892083943f, 0.893466771f, 0.894841135f, 0.896207094f, 0.89756453f,
		0.898913622f, 0.90025419f, 0.901586294f, 0.902909875f, 0.904224992f, 0.905531526f, 0.906829596f,
		0.908119082f, 0.909400046f, 0.910672367f, 0.911936164f, 0.913191378f, 0.914438009f, 0.915675938f,
		0.916905344f, 0.918125987f, 0.919338107f, 0.920541465f, 0.92173624f, 0.922922254f, 0.924099624f,
		0.925268233f, 0.926428199f, 0.927579403f, 0.928721845f, 0.929855525f, 0.930980504f, 0.93209666f,
		0.933204114f, 0.934302688f, 0.935392499f, 0.936473429f, 0.937545657f, 0.938608944f, 0.93966347f,
		0.940709054f, 0.941745877f, 0.942773759f, 0.94379276f, 0.94480288f, 0.945804119f, 0.946796417f,
		0.947779834f, 0.948754311f, 0.949719846f, 0.950676382f, 0.951624036f, 0.95256263f, 0.953492343f,
		0.954413056f, 0.955324769f, 0.956227422f, 0.957121134f, 0.958005786f, 0.958881497f, 0.959748089f,
		0.960605681f, 0.961454153f, 0.962293625f, 0.963124037f, 0.963945389f, 0.964757621f, 0.965560734f,
		0.966354787f, 0.967139781f, 0.967915595f, 0.968682349f, 0.969439924f, 0.970188379f, 0.970927656f,
		0.971657872f, 0.97237885f, 0.973090708f, 0.973793387f, 0.974486947f, 0.975171268f, 0.97584641f,
		0.976512372f, 0.977169096f, 0.977816641f, 0.978455007f, 0.979084074f, 0.979703963f, 0.980314612f,
		0.980916083f, 0.981508195f, 0.982091188f, 0.982664824f, 0.98322928f, 0.983784437f, 0.984330297f,
		0.984866917f, 0.985394239f, 0.985912323f, 0.986421049f, 0.986920536f, 0.987410724f, 0.987891555f,
		0.988363147f, 0.988825381f, 0.989278316f, 0.989721894f, 0.990156233f, 0.990581155f, 0.990996778f,
		0.991403043f, 0.99180001f, 0.992187619f, 0.99256587f, 0.992934763f, 0.993294358f, 0.993644536f,
		0.993985355f, 0.994316816f, 0.99463892f, 0.994951606f, 0.995254993f, 0.995548964f, 0.995833576f,
		0.99610877f, 0.996374607f, 0.996631026f, 0.996878088f, 0.997115731f, 0.997344017f, 0.997562885f,
		0.997772336f, 0.997972369f, 0.998163044f, 0.998344302f, 0.998516202f, 0.998678625f, 0.99883163f,
		0.998975277f, 0.999109507f, 0.999234319f, 0.999349713f, 0.999455631f, 0.99955219f, 0.999639332f,
		0.999717057f, 0.999785364f, 0.999844253f, 0.999893725f, 0.999933779f, 0.999964356f, 0.999985576f,
		0.999997377f, 0.999999702f, 0.999992669f, 0.999976158f, 0.99995023f, 0.999914885f, 0.999870181f,
		0.999816f, 0.999752402f, 0.999679387f, 0.999596953f, 0.999505103f, 0.999403834f, 0.999293149f,
		0.999173105f, 0.999043584f, 0.998904645f, 0.998756289f, 0.998598576f, 0.998431444f, 0.998254895f,
		0.998068929f, 0.997873545f, 0.997668743f, 0.997454584f, 0.997231007f, 0.996998072f, 0.996755719f,
		0.996504009f, 0.996242821f, 0.995972335f, 0.995692432f, 0.995403171f, 0.995104492f, 0.994796455f,
		0.994479001f, 0.994152248f, 0.993816078f, 0.993470609f, 0.993115723f, 0.992751479f, 0.992377937f,
		0.991994977f, 0.991602719f, 0.991201103f, 0.990790129f, 0.990369856f, 0.989940226f, 0.989501297f,
		0.989053011f, 0.988595426f, 0.988128483f, 0.987652302f, 0.987166762f, 0.986671984f, 0.986167848f,
		0.985654473f, 0.985131741f, 0.984599769f, 0.984058499f, 0.983507991f, 0.982948184f, 0.982379138f,
		0.981800854f, 0.981213331f, 0.98061651f, 0.98001045f, 0.979395151f, 0.978770673f, 0.978136957f,
		0.977494061f, 0.976841867f, 0.976180553f, 0.975510001f, 0.97483027f, 0.9741413f, 0.97344321f,
		0.972735941f, 0.972019494f, 0.971293926f, 0.97055918f, 0.969815254f, 0.969062269f, 0.968300104f,
		0.96752882f, 0.966748416f, 0.965958953f, 0.96516031f, 0.964352608f, 0.963535845f, 0.962709963f,
		0.961875021f, 0.961031079f, 0.960177958f, 0.959315896f, 0.958444774f, 0.957564592f, 0.95667541f,
		0.955777228f, 0.954869986f, 0.953953803f, 0.953028619f, 0.952094495f, 0.951151311f, 0.950199246f,
		0.949238181f, 0.948268175f, 0.947289228f, 0.946301401f, 0.945304632f, 0.944298983f, 0.943284392f,
		0.942260921f, 0.941228569f, 0.940187395f, 0.93913728f, 0.938078403f, 0.937010646f, 0.935934067f,
		0.934848666f, 0.933754504f, 0.93265146f, 0.931539714f, 0.930419147f, 0.929289818f, 0.928151727f,
		0.927004874f, 0.925849319f, 0.924685061f, 0.923512042f, 0.92233032f, 0.921139956f, 0.919940889f,
		0.91873312f, 0.917516768f, 0.916291714f, 0.915058076f, 0.913815737f, 0.912564874f, 0.911305368f,
		0.910037279f, 0.908760607f, 0.907475412f, 0.906181693f, 0.904879272f, 0.903568447f, 0.902249157f,
		0.900921345f, 0.899584949f, 0.898240149f, 0.896886885f, 0.895525217f, 0.894154966f, 0.89277637f,
		0.89138943f, 0.889994085f, 0.888590217f, 0.887178063f, 0.885757625f, 0.884328783f, 0.882891476f,
		0.881446004f, 0.879992247f, 0.878530145f, 0.877059698f, 0.875581086f, 0.874094248f, 0.872599125f,
		0.871095717f, 0.869584203f, 0.868064463f, 0.866536558f, 0.865000367f, 0.86345619f, 0.861903846f,
		0.860343397f, 0.858774662f, 0.857198f, 0.855613291f, 0.854020476f, 0.852419496f, 0.850810647f,
		0.849193752f, 0.84756887f, 0.845935881f, 0.844295025f, 0.842646182f, 0.84098947f, 0.839324653f,
		0.837652087f, 0.835971653f, 0.834283292f, 0.832586944f, 0.830882907f, 0.829171062f, 0.827451348f,
		0.825723767f, 0.823988497f, 0.822245479f, 0.820494711f, 0.818736076f, 0.816969872f, 0.815195978f,
		0.813414395f, 0.811625004f, 0.809828103f, 0.808023572f, 0.806211412f, 0.804391503f, 0.802564204f,
		0.800729334f, 0.798886895f, 0.797036827f, 0.795179367f, 0.793314397f, 0.791441977f, 0.789561987f,
		0.787674665f, 0.785779893f, 0.78387779f, 0.781968117f, 0.780051231f, 0.778126955f, 0.776195407f,
		0.774256349f, 0.772310138f, 0.770356715f, 0.76839602f, 0.766427875f, 0.764452696f, 0.762470305f,
		0.760480762f, 0.758483887f, 0.756480038f, 0.754469037f, 0.752450883f, 0.750425577f, 0.748393297f,
		0.746353984f, 0.744307637f, 0.742254078f, 0.740193725f, 0.738126397f, 0.736052096f, 0.733970761f,
		0.731882632f, 0.729787588f, 0.72768569f, 0.725576758f, 0.723461151f, 0.721338749f, 0.719209552f,
		0.717073381f, 0.714930654f, 0.712781191f, 0.710624993f, 0.70846194f, 0.706292391f, 0.704116166f,
		0.701933324f, 0.699743688f, 0.697547615f, 0.695344985f, 0.693135798f, 0.690919936f, 0.688697696f,
		0.686469018f, 0.684233844f, 0.681992054f, 0.679744005f, 0.677489579f, 0.675228715f, 0.672961354f,
		0.670687795f, 0.668407977f, 0.66612184f, 0.663829207f, 0.661530554f, 0.659225643f, 0.656914473f,
		0.654596984f, 0.652273536f, 0.649943888f, 0.647608161f, 0.645266116f, 0.642918169f, 0.640564203f,
		0.638204217f, 0.635837972f, 0.633465946f, 0.631087959f, 0.628704011f, 0.626313984f, 0.623918235f,
		0.621516585f, 0.619109094f, 0.616695583f, 0.614276409f, 0.611851513f, 0.609420776f, 0.606984198f,
		0.604542017f, 0.602094173f, 0.599640667f, 0.59718132f, 0.594716489f, 0.592246115f, 0.589770138f,
		0.58728838f, 0.584801316f, 0.582308769f, 0.579810679f, 0.577306986f, 0.574797988f, 0.572283626f,
		0.569763839f, 0.56723851f, 0.564708054f, 0.562172234f, 0.559631109f, 0.55708456f, 0.554532945f,
		0.551976085f, 0.549414039f, 0.546846628f, 0.544274271f, 0.541696727f, 0.539114177f, 0.536526263f,
		0.53393352f, 0.531335771f, 0.528732955f, 0.526125014f, 0.523512304f, 0.520894647f, 0.518272102f,
		0.515644431f, 0.513012171f, 0.510375023f, 0.507733107f, 0.505086184f, 0.502434671f, 0.499778479f,
		0.497117549f, 0.494451731f, 0.491781473f, 0.489106566f, 0.486427069f, 0.483742774f, 0.481054127f,
		0.478360951f, 0.475663275f, 0.472960889f, 0.470254272f, 0.467543215f, 0.464827776f, 0.462107718f,
		0.459383518f, 0.456654996f, 0.453922182f, 0.451184869f, 0.448443532f, 0.445697963f, 0.442948192f,
		0.440194041f, 0.437435925f, 0.434673727f, 0.431907415f, 0.429136813f, 0.426362395f, 0.423583955f,
		0.42080152f, 0.418014914f, 0.415224582f, 0.412430346f, 0.409632206f, 0.406830013f, 0.404024184f,
		0.40121457f, 0.398401141f, 0.395583779f, 0.39276287f, 0.389938295f, 0.387110025f, 0.38427791f,
		0.381442368f, 0.37860325f, 0.375760555f, 0.372914106f, 0.370064348f, 0.367211133f, 0.364354432f,
		0.361494094f, 0.358630568f, 0.355763674f, 0.352893412f, 0.350019604f, 0.347142696f, 0.34426257f,
		0.341379166f, 0.338492334f, 0.335602522f, 0.332709551f, 0.329813451f, 0.326914042f, 0.324011743f,
		0.321106404f, 0.318198055f, 0.315286458f, 0.312372118f, 0.309454858f, 0.306534678f, 0.303611368f,
		0.300685436f, 0.297756642f, 0.294825077f, 0.291890502f, 0.288953394f, 0.286013573f, 0.283071041f,
		0.280125618f, 0.277177811f, 0.274227351f, 0.271274328f, 0.268318534f, 0.265360415f, 0.262399822f,
		0.259436727f, 0.256470978f, 0.253503054f, 0.250532717f, 0.247560039f, 0.244584784f, 0.241607457f,
		0.238627851f, 0.235646009f, 0.232661709f, 0.229675442f, 0.226687029f, 0.223696455f, 0.220703557f,
		0.217708811f, 0.214712009f, 0.211713195f, 0.208712146f, 0.205709368f, 0.202704638f, 0.199698016f,
		0.196689263f, 0.193678901f, 0.19066672f, 0.187652737f, 0.184636742f, 0.181619242f, 0.178600043f,
		0.17557916f, 0.172556385f, 0.16953221f, 0.166506454f, 0.16347912f, 0.160450011f, 0.157419622f,
		0.154387757f, 0.151354432f, 0.148319453f, 0.145283312f, 0.142245799f, 0.139206931f, 0.136166543f,
		0.133125082f, 0.130082384f, 0.127038449f, 0.123993091f, 0.120946802f, 0.117899366f, 0.114850819f,
		0.111800961f, 0.108750276f, 0.105698578f, 0.102645874f, 0.0995919704f, 0.0965373665f, 0.0934818536f,
		0.0904254541f, 0.0873679742f, 0.0843098983f, 0.0812510327f, 0.078191407f, 0.0751307979f, 0.0720697269f,
		0.0690079704f, 0.0659455657f, 0.0628823042f, 0.0598186888f, 0.0567545071f, 0.053689789f, 0.0506243296f,
		0.0475586317f, 0.0444924869f, 0.041425921f, 0.0383587256f, 0.0352914073f, 0.0322237574f, 0.0291558038f,
		0.0260873362f, 0.0230188612f, 0.0199501701f, 0.0168812927f, 0.0138120158f, 0.0107428469f, 0.00767357787f,
		0.00460423576f, 0.00153461238f, -0.00153478724f, -0.00460417243f, -0.00767351408f, -0.010743022f, -0.0138121899f,
		-0.0168812275f, -0.0199501067f, -0.0230190363f, -0.0260875113f, -0.0291557387f, -0.032223694f, -0.0352915823f,
		-0.0383589007f, -0.041425854f, -0.0444924198f, -0.0475588068f, -0.0506245047f, -0.0536897257f, -0.0567544438f,
		-0.0598188639f, -0.062882483f, -0.0659455061f, -0.0690079108f, -0.0720698982f, -0.0751309767f, -0.07819134f,
		-0.081250973f, -0.0843100771f, -0.0873681456f, -0.0904253945f, -0.0934817865f, -0.0965375379f, -0.0995921418f,
		-0.102645814f, -0.105698511f, -0.108750217f, -0.111801133f, -0.11485076f, -0.117899306f, -0.120946735f,
		-0.123993263f, -0.127038389f, -0.130082324f, -0.133125022f, -0.136166707f, -0.139206871f, -0.142245725f,
		-0.145283237f, -0.148319632f, -0.151354373f, -0.154387698f, -0.157419562f, -0.16045019f, -0.16347906f,
		-0.166506395f, -0.16953215f, -0.172556549f, -0.175579101f, -0.178599983f, -0.181619182f, -0.184636921f,
		-0.187652662f, -0.190666646f, -0.193678841f, -0.196689442f, -0.199697956f, -0.202704579f, -0.205709308f,
		-0.208712325f, -0.211713135f, -0.214711949f, -0.217708752f, -0.220703736f, -0.223696396f, -0.226686954f,
		-0.229675382f, -0.232661873f, -0.23564595f, -0.238627791f, -0.241607398f, -0.244584948f, -0.247559965f,
		-0.250532657f, -0.253502995f, -0.256471157f, -0.259436667f, -0.262399763f, -0.265360355f, -0.268318683f,
		-0.271274269f, -0.274227291f, -0.277177721f, -0.280125797f, -0.283070982f, -0.286013514f, -0.288953334f,
		-0.291890651f, -0.294825017f, -0.297756582f, -0.300685346f, -0.303611517f, -0.306534618f, -0.309454799f,
		-0.312372059f, -0.315286636f, -0.318197995f, -0.321106344f, -0.324011683f, -0.326914191f, -0.329813391f,
		-0.332709491f, -0.335602462f, -0.338492483f, -0.341379106f, -0.344262511f, -0.347142637f, -0.350019753f,
		-0.352893353f, -0.355763614f, -0.358630508f, -0.361494273f, -0.364354372f, -0.367211074f, -0.370064318f,
		-0.372914255f, -0.375760496f, -0.37860319f, -0.381442308f, -0.384278059f, -0.387109965f, -0.389938235f,
		-0.39276284f, -0.395583928f, -0.398401082f, -0.40121451f, -0.404024124f, -0.406830162f, -0.409632146f,
		-0.412430286f, -0.415224522f, -0.418015093f, -0.420801461f, -0.423583895f, -0.426362336f, -0.429136992f,
		-0.431907356f, -0.434673667f, -0.437435865f, -0.44019419f, -0.442948133f, -0.445697904f, -0.448443472f,
		-0.451185048f, -0.453922123f, -0.456654966f, -0.459383458f, -0.462107867f, -0.464827716f, -0.467543155f,
		-0.470254213f, -0.472961038f, -0.475663215f, -0.478360891f, -0.481054068f, -0.483742923f, -0.486427009f,
		-0.489106536f, -0.491781414f, -0.49445188f, -0.49711749f, -0.49977842f, -0.502434611f, -0.505086303f,
		-0.507733047f, -0.510374963f, -0.513012111f, -0.51564461f, -0.518272042f, -0.520894587f, -0.523512244f,
		-0.526125133f, -0.528732896f, -0.531335711f, -0.533933461f, -0.536526382f, -0.539114118f, -0.541696727f,
		-0.544274211f, -0.546846807f, -0.549413979f, -0.551976025f, -0.554532886f, -0.557084739f, -0.559631109f,
		-0.562172174f, -0.564707994f, -0.567238688f, -0.569763839f, -0.572283566f, -0.574797988f, -0.577307105f,
		-0.579810619f, -0.58230871f, -0.584801316f, -0.587288558f, -0.589770079f, -0.592246056f, -0.59471643f,
		-0.597181439f, -0.599640608f, -0.602094114f, -0.604541957f, -0.606984317f, -0.609420776f, -0.611851454f,
		-0.61427635f, -0.616695702f, -0.619109035f, -0.621516526f, -0.623918176f, -0.626314104f, -0.628703952f,
		-0.631087899f, -0.633465886f, -0.635838091f, -0.638204157f, -0.640564144f, -0.642918169f, -0.645266235f,
		-0.647608101f, -0.649943829f, -0.652273476f, -0.654597163f, -0.656914473f, -0.659225583f, -0.661530495f,
		-0.663829327f, -0.666121781f, -0.668407917f, -0.670687795f, -0.672961473f, -0.675228655f, -0.677489519f,
		-0.679743946f, -0.681992173f, -0.684233785f, -0.686468959f, -0.688697696f, -0.690920055f, -0.693135798f,
		-0.695344925f, -0.697547555f, -0.699743807f, -0.701933265f, -0.704116106f, -0.706292331f, -0.708462059f,
		-0.710624933f, -0.712781131f, -0.714930594f, -0.7170735f, -0.719209492f, -0.721338689f, -0.723461092f,
		-0.725576878f, -0.72768563f, -0.729787529f, -0.731882572f, -0.733970881f, -0.736052096f, -0.738126338f,
		-0.740193665f, -0.742254198f, -0.744307578f, -0.746353924f, -0.748393238f, -0.750425637f, -0.752450883f,
		-0.754468977f, -0.756479979f, -0.758484006f, -0.760480702f, -0.762470305f, -0.764452636f, -0.766427815f,
		-0.768395782f, -0.770356834f, -0.772310257f, -0.774256468f, -0.776195347f, -0.778126895f, -0.780051172f,
		-0.781968057f, -0.783877611f, -0.785780013f, -0.787674785f, -0.789562106f, -0.791441977f, -0.793314397f,
		-0.795179307f, -0.797036767f, -0.798886716f, -0.800729454f, -0.802564323f, -0.804391623f, -0.806211352f,
		-0.808023512f, -0.809828043f, -0.811624944f, -0.813414216f, -0.815196037f, -0.816969991f, -0.818736196f,
		-0.820494652f, -0.822245419f, -0.823988497f, -0.825723708f, -0.827451229f, -0.829171181f, -0.830883026f,
		-0.832587063f, -0.834283292f, -0.835971594f, -0.837652087f, -0.839324653f, -0.840989292f, -0.842646301f,
		-0.844295084f, -0.845935941f, -0.84756881f, -0.849193692f, -0.850810587f, -0.852419496f, -0.854020298f,
		-0.855613351f, -0.857198119f, -0.858774781f, -0.860343337f, -0.861903787f, -0.86345613f, -0.865000367f,
		-0.866536438f, -0.868064523f, -0.869584262f, -0.871095777f, -0.872599125f, -0.874094188f, -0.875581086f,
		-0.877059698f, -0.878530025f, -0.879992306f, -0.881446123f, -0.882891595f, -0.884328723f, -0.885757565f,
		-0.887178063f, -0.888590157f, -0.889993906f, -0.891389489f, -0.892776489f, -0.894155025f, -0.895525157f,
		-0.896886885f, -0.898240089f, -0.899584889f, -0.900921226f, -0.902249217f, -0.903568566f, -0.904879332f,
		-0.906181633f, -0.907475352f, -0.908760548f, -0.91003722f, -0.911305249f, -0.912564933f, -0.913815796f,
		-0.915058076f, -0.916291714f, -0.917516708f, -0.91873312f, -0.919940829f, -0.921139836f, -0.922330439f,
		-0.923512101f, -0.924685061f, -0.925849319f, -0.927004874f, -0.928151667f, -0.929289758f, -0.930419028f,
		-0.931539774f, -0.93265152f, -0.933754504f, -0.934848666f, -0.935934067f, -0.937010646f, -0.938078344f,
		-0.93913722f, -0.940187454f, -0.941228628f, -0.942260921f, -0.943284392f, -0.944298923f, -0.945304573f,
		-0.946301341f, -0.947289169f, -0.948268235f, -0.949238241f, -0.950199246f, -0.951151311f, -0.952094436f,
		-0.953028619f, -0.953953743f, -0.954869926f, -0.955777287f, -0.95667547f, -0.957564652f, -0.958444774f,
		-0.959315896f, -0.960177958f, -0.96103102f, -0.961874962f, -0.962710023f, -0.963535845f, -0.964352667f,
		-0.96516031f, -0.965958893f, -0.966748416f, -0.96752876f, -0.968300045f, -0.969062328f, -0.969815314f,
		-0.97055918f, -0.971293926f, -0.972019494f, -0.972735941f, -0.97344321f, -0.9741413f, -0.97483027f,
		-0.975510001f, -0.976180553f, -0.976841867f, -0.977494001f, -0.978136957f, -0.978770673f, -0.979395151f,
		-0.980010509f, -0.98061651f, -0.981213331f, -0.981800854f, -0.982379138f, -0.982948184f, -0.983507991f,
		-0.984058499f, -0.984599829f, -0.9851318f, -0.985654473f, -0.986167848f, -0.986671984f, -0.987166762f,
		-0.987652302f, -0.988128483f, -0.988595426f, -0.989053011f, -0.989501297f, -0.989940226f, -0.990369856f,
		-0.990790129f, -0.991201103f, -0.991602719f, -0.991995037f, -0.992377937f, -0.992751539f, -0.993115723f,
		-0.993470609f, -0.993816078f, -0.994152248f, -0.994479001f, -0.994796455f, -0.995104492f, -0.995403171f,
		-0.995692432f, -0.995972335f, -0.996242821f, -0.996503949f, -0.996755719f, -0.996998072f, -0.997231066f,
		-0.997454584f, -0.997668743f, -0.997873545f, -0.998068869f, -0.998254836f, -0.998431385f, -0.998598576f,
		-0.998756349f, -0.998904645f, -0.999043584f, -0.999173045f, -0.999293149f, -0.999403834f, -0.999505103f,
		-0.999596953f, -0.999679387f, -0.999752402f, -0.999816f, -0.999870181f, -0.999914885f, -0.99995023f,
		-0.999976158f, -0.999992669f, -0.999999702f, -0.999997377f, -0.999985576f, -0.999964356f, -0.999933779f,
		-0.999893725f, -0.999844253f, -0.999785364f, -0.999717057f, -0.999639332f, -0.99955219f, -0.999455631f,
		-0.999349713f, -0.999234319f, -0.999109507f, -0.998975277f, -0.99883163f, -0.998678625f, -0.998516142f,
		-0.998344302f, -0.998163044f, -0.997972429f, -0.997772336f, -0.997562826f, -0.997343957f, -0.997115731f,
		-0.996878088f, -0.996631026f, -0.996374607f, -0.99610877f, -0.995833576f, -0.995548964f, -0.995254993f,
		-0.994951606f, -0.99463892f, -0.994316816f, -0.993985355f, -0.993644536f, -0.993294358f, -0.992934763f,
		-0.99256587f, -0.992187619f, -0.99180001f, -0.991403103f, -0.990996778f, -0.990581214f, -0.990156233f,
		-0.989721894f, -0.989278316f, -0.988825381f, -0.988363147f, -0.987891555f, -0.987410724f, -0.986920536f,
		-0.986421108f, -0.985912263f, -0.985394239f, -0.984866917f, -0.984330297f, -0.983784437f, -0.98322928f,
		-0.982664883f, -0.982091188f, -0.981508195f, -0.980916023f, -0.980314612f, -0.979703963f, -0.979084074f,
		-0.978455007f, -0.977816701f, -0.977169156f, -0.976512313f, -0.97584635f, -0.975171268f, -0.974486947f,
		-0.973793447f, -0.973090768f, -0.97237891f, -0.971657932f, -0.970927656f, -0.97018832f, -0.969439864f,
		-0.968682289f, -0.967915595f, -0.967139781f, -0.966354847f, -0.965560794f, -0.964757562f, -0.963945329f,
		-0.963124037f, -0.962293625f, -0.961454153f, -0.960605681f, -0.959748149f, -0.958881497f, -0.958005726f,
		-0.957121074f, -0.956227422f, -0.955324709f, -0.954413056f, -0.953492343f, -0.95256269f, -0.951624095f,
		-0.950676322f, -0.949719787f, -0.948754251f, -0.947779834f, -0.946796477f, -0.945804179f, -0.94480294f,
		-0.94379282f, -0.9427737f, -0.941745818f, -0.940709054f, -0.93966347f, -0.938609004f, -0.937545657f,
		-0.936473548f, -0.935392559f, -0.934302628f, -0.933204055f, -0.93209666f, -0.930980504f, -0.929855585f,
		-0.928721905f, -0.927579463f, -0.926428258f, -0.925268173f, -0.924099565f, -0.922922254f, -0.92173624f,
		-0.920541525f, -0.919338107f, -0.918126106f, -0.916905403f, -0.915675879f, -0.91443789f, -0.913191319f,
		-0.911936164f, -0.910672426f, -0.909400046f, -0.908119142f, -0.906829655f, -0.905531466f, -0.904224932f,
		-0.902909815f, -0.901586294f, -0.90025419f, -0.898913682f, -0.89756465f, -0.896207154f, -0.894841015f,
		-0.893466711f, -0.892083943f, -0.89069277f, -0.889293194f, -0.887885273f, -0.886468947f, -0.885044277f,
		-0.883611083f, -0.882169783f, -0.880720139f, -0.879262209f, -0.877795994f, -0.876321495f, -0.874838769f,
		-0.873347759f, -0.871848345f, -0.870340943f, -0.868825316f, -0.867301464f, -0.865769506f, -0.864229381f,
		-0.862681091f, -0.861124694f, -0.859559953f, -0.857987344f, -0.856406629f, -0.854817867f, -0.853220999f,
		-0.851616144f, -0.850003302f, -0.848382413f, -0.84675324f, -0.845116377f, -0.843471527f, -0.84181881f,
		-0.840158105f, -0.838489473f, -0.836812913f, -0.835128546f, -0.833436012f, -0.831735909f, -0.830027938f,
		-0.828312159f, -0.826588571f, -0.824857175f, -0.823118031f, -0.821371138f, -0.819616258f, -0.817853868f,
		-0.816083848f, -0.81430608f, -0.812520623f, -0.810727596f, -0.80892688f, -0.807118535f, -0.805302322f,
		-0.803478777f, -0.801647663f, -0.799809039f, -0.797962844f, -0.79610914f, -0.794247925f, -0.79237926f,
		-0.790502787f, -0.788619161f, -0.786728144f, -0.784829736f, -0.782923877f, -0.781010687f, -0.779090106f,
		-0.777162194f, -0.775226653f, -0.773284137f, -0.771334291f, -0.769377172f, -0.767412841f, -0.765441298f,
		-0.763462484f, -0.761476517f, -0.759483099f, -0.757482767f, -0.755475342f, -0.753460765f, -0.751439154f,
		-0.749410391f, -0.747374594f, -0.745331764f, -0.743281603f, -0.741224706f, -0.739160895f, -0.737090051f,
		-0.735012293f, -0.73292762f, -0.730836034f, -0.728737593f, -0.726631939f, -0.72451973f, -0.722400725f,
		-0.720274925f, -0.718142331f, -0.716002941f, -0.713856876f, -0.711704016f, -0.709544122f, -0.707377911f,
		-0.705205023f, -0.70302552f, -0.700839341f, -0.698646605f, -0.696447253f, -0.694241345f, -0.692028522f,
		-0.689809561f, -0.687584102f, -0.685352147f, -0.683113754f, -0.680868924f, -0.678617716f, -0.676360071f,
		-0.67409569f, -0.67182529f, -0.669548631f, -0.667265594f, -0.664976299f, -0.662680745f, -0.660378993f,
		-0.658070922f, -0.655756354f, -0.653435946f, -0.651109397f, -0.64877671f, -0.646437943f, -0.644093037f,
		-0.641742051f, -0.639385045f, -0.637021661f, -0.634652615f, -0.632277608f, -0.629896641f, -0.627509773f,
		-0.625116944f, -0.622718215f, -0.620313644f, -0.617902875f, -0.615486622f, -0.613064587f, -0.610636771f,
		-0.608203232f, -0.605763912f, -0.60331893f, -0.600868225f, -0.5984115f, -0.595949531f, -0.593481958f,
		-0.591008723f, -0.588530004f, -0.586045682f, -0.583555877f, -0.581060529f, -0.578559339f, -0.576053083f,
		-0.573541403f, -0.571024358f, -0.56850189f, -0.565974057f, -0.563440919f, -0.560902476f, -0.558358312f,
		-0.555809319f, -0.553255081f, -0.550695658f, -0.548130989f, -0.545561194f, -0.542986274f, -0.540406227f,
		-0.537820637f, -0.535230458f, -0.532635212f, -0.5300349f, -0.52742964f, -0.524819374f, -0.52220422f,
		-0.519584119f, -0.516958714f, -0.514328837f, -0.511694133f, -0.509054601f, -0.506410241f, -0.503761172f,
		-0.501107275f, -0.49844873f, -0.495785058f, -0.493117124f, -0.490444541f, -0.487767339f, -0.485085517f,
		-0.482399166f, -0.479708254f, -0.477012813f, -0.474312454f, -0.471608073f, -0.46889922f, -0.466185957f,
		-0.463468313f, -0.460746288f, -0.458019942f, -0.455289274f, -0.452553868f, -0.449814647f, -0.447071195f,
		-0.44432351f, -0.441571653f, -0.438815624f, -0.436055481f, -0.433291227f, -0.430522442f, -0.427750021f,
		-0.424973607f, -0.42219317f, -0.419408739f, -0.416620374f, -0.413828105f, -0.411031902f, -0.408231407f,
		-0.405427486f, -0.402619779f, -0.399808258f, -0.396992952f, -0.39417392f, -0.391351193f, -0.388524771f,
		-0.385694236f, -0.382860512f, -0.380023181f, -0.377182275f, -0.374337792f, -0.371489823f, -0.368638307f,
		-0.365783364f, -0.362924516f, -0.360062689f, -0.357197464f, -0.354328871f, -0.35145694f, -0.348581702f,
		-0.345703185f, -0.342821419f, -0.339935958f, -0.337047726f, -0.334156364f, -0.331261814f, -0.328364164f,
		-0.325463414f, -0.322559595f, -0.319652736f, -0.31674242f, -0.313829571f, -0.310913771f, -0.307995051f,
		-0.30507341f, -0.302148879f, -0.299221545f, -0.296291351f, -0.293357939f, -0.290422201f, -0.287483722f,
		-0.284542561f, -0.281598687f, -0.278652191f, -0.275703043f, -0.272751302f, -0.26979655f, -0.266839713f,
		-0.263880342f, -0.260918498f, -0.25795421f, -0.254987448f, -0.252018332f, -0.249046817f, -0.246072501f,
		-0.243096322f, -0.240117848f, -0.237137124f, -0.234154165f, -0.231169f, -0.228181645f, -0.225192159f,
		-0.222200066f, -0.219206363f, -0.216210589f, -0.213212773f, -0.210212946f, -0.207211152f, -0.204207391f,
		-0.201201707f, -0.198193669f, -0.195184231f, -0.192172945f, -0.189159855f, -0.186144993f, -0.183128357f,
		-0.180110008f, -0.177089959f, -0.174067765f, -0.171044409f, -0.168019444f, -0.164992899f, -0.161964789f,
		-0.158935159f, -0.155904025f, -0.15287143f, -0.149836913f, -0.146801472f, -0.14376463f, -0.140726447f,
		-0.137686923f, -0.134646118f, -0.131604046f, -0.128560722f, -0.125515714f, -0.122470006f, -0.119423136f,
		-0.116375141f, -0.113326058f, -0.110275902f, -0.107224703f, -0.104172498f, -0.10111884f, -0.0980646983f,
		-0.0950096324f, -0.0919536725f, -0.0888968483f, -0.0858391896f, -0.0827807188f, -0.0797214657f, -0.0766609907f,
		-0.0736002624f, -0.0705388486f, -0.0674767643f, -0.0644140467f, -0.0613507181f, -0.0582868159f, -0.055222366f,
		-0.052156914f, -0.0490914509f, -0.0460255258f, -0.0429591648f, -0.0398924015f, -0.0368252583f, -0.0337577723f,
		-0.0306899678f, -0.0276213959f, -0.02455304f, -0.0214844551f, -0.0184156653f, -0.0153467031f, -0.0122775966f,
		-0.00920837466f, -0.00613906514f, -0.00306922104f, 0.f
};